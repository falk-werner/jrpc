/*
 * JRPC - Yet another JSON-RPC server based on libwebsockets
 *  <https://github.com/falk-werner/jrpc>
 *
 * Copyright (c) 2019 Falk Werner
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "jrpc/protocol.h"
#include "jrpc/connection_intern.h"
#include "jrpc/message.h"
#include "jrpc/util.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

static void jrpc_protocol_process(
    struct jrpc_protocol * protocol,
    struct jrpc_connection * connection,
    char const * buffer,
    size_t length
)
{
    json_t * request = json_loadb(buffer, length, 0, NULL);
    if (NULL != request)
    {
        json_t * name_holder = json_object_get(request, "method");
        json_t * params = json_object_get(request, "params");
        json_t * id_holder = json_object_get(request, "id");

        if ((NULL != name_holder) && (json_is_string(name_holder)) &&
            (NULL != params) && (json_is_array(params)))
        {
            char const * method_name = json_string_value(name_holder);

            if ((NULL != id_holder) && (json_is_integer(id_holder)))
            {
                int id = json_integer_value(id_holder);
                protocol->onmethod(connection, method_name, params, id);
            }
            else
            {
                protocol->onnotify(connection, method_name, params);
            }
            
        }

        json_decref(request);
    }
}

static int jrpc_protocol_callback(
    struct lws * wsi,
    enum lws_callback_reasons reason,
    void * user,
    void * in,
    size_t length
)
{
    struct lws_protocols const * lws_protocol = lws_get_protocol(wsi);
    if (NULL == lws_protocol)
    {
        return 0;
    }

    struct jrpc_protocol * protocol = lws_protocol->user;
    struct jrpc_connection * connection = user;

    switch (reason)
    {
    case LWS_CALLBACK_PROTOCOL_INIT:
        {
            lws_sock_file_fd_type fd;
            fd.sockfd = protocol->fd[0];
            lws_adopt_descriptor_vhost(lws_get_vhost(wsi), LWS_ADOPT_RAW_FILE_DESC, fd, lws_protocol->name, NULL);
        }
        break;
    case LWS_CALLBACK_ESTABLISHED:
        if (NULL != connection)
        {
            jrpc_connection_init(connection, protocol->server, wsi);
            protocol->onconnected(connection);
        }
        break;
    case LWS_CALLBACK_CLOSED:
        if (NULL != connection)
        {
            protocol->ondisconnected(connection);
            jrpc_connection_cleanup(connection);
        }
        break;        
    case LWS_CALLBACK_RECEIVE:
        if (NULL != connection)
        {
            jrpc_protocol_process(protocol, connection, in, length);
        }
        break;
    case LWS_CALLBACK_SERVER_WRITEABLE:
        if ((NULL != connection) && (!jrpc_queue_is_empty(&connection->messages)))
        {
            struct jrpc_message * message = jrpc_queue_dequeue(&connection->messages);
            lws_write(wsi, (unsigned char *) message->data, message->length, LWS_WRITE_TEXT);
            jrpc_message_dispose(message);
        }
        break;
    case LWS_CALLBACK_RAW_RX_FILE:
        {
            char temp;
            read(protocol->fd[0], &temp, 1);
        }
        break;
    default:
        break;
    }

    if ((NULL != connection) && (!jrpc_queue_is_empty(&connection->messages)))
    {
        lws_callback_on_writable(wsi);
    }

    return 0;
}

static void jrpc_default_onmethod(
    struct jrpc_connection * connection,
    char const * JRPC_UNUSED_PARAM(method_name),
    json_t * JRPC_UNUSED_PARAM(params),
    int id)
{
    jrpc_respond_error(connection, -1, "not implemented", id);
}

static void jrpc_default_onnotify(
    struct jrpc_connection * JRPC_UNUSED_PARAM(connection),
    char const * JRPC_UNUSED_PARAM(method_name),
    json_t * JRPC_UNUSED_PARAM(content))
{
    // empty
}

static void jrpc_default_onconnected(
    struct jrpc_connection * JRPC_UNUSED_PARAM(connection))
{
    // empty
}

static void jrpc_default_ondisconnected(
    struct jrpc_connection * JRPC_UNUSED_PARAM(connection))
{
    // empty
}


void jrpc_server_protocol_init(
    struct jrpc_protocol * protocol,
    struct jrpc_server * server)
{
    protocol->server = server;
    protocol->onmethod = &jrpc_default_onmethod;
    protocol->onnotify = &jrpc_default_onnotify;
    protocol->onconnected = &jrpc_default_onconnected;
    protocol->ondisconnected = &jrpc_default_ondisconnected;

    socketpair(AF_UNIX, SOCK_DGRAM, 0, protocol->fd);
}

void jrpc_protocol_cleanup(
    struct jrpc_protocol * JRPC_UNUSED_PARAM(protocol))
{
    close(protocol->fd[0]);
    close(protocol->fd[1]);
}

void jrpc_protocol_init_lws(
    struct jrpc_protocol * protocol,
    struct lws_protocols * lws_protocol
)
{
    lws_protocol->callback = &jrpc_protocol_callback;
    lws_protocol->per_session_data_size = sizeof(struct jrpc_connection);
    lws_protocol->user = protocol;
}

extern void jrpc_protocol_wakeup(
    struct jrpc_protocol * protocol)
{
    char value = 42;
    write(protocol->fd[1], &value, 1);
}
