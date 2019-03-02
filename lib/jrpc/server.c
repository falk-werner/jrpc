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

#include "jrpc/server.h"
#include "jrpc/protocol.h"

#include <libwebsockets.h>

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define JRPC_DISABLE_LWS_LOG 0
#define JRPC_SERVER_PROTOCOL_COUNT 3
#define JRPC_SERVER_TIMEOUT (1 * 1000)

#define JRPC_SERVER_DEFAULT_PORT 8080
#define JRPC_SERVER_DEFAULT_PROTOCOL_NAME ("jrpc")

struct jrpc_server
{
    struct jrpc_protocol protocol;
    struct lws_protocols ws_protocols[JRPC_SERVER_PROTOCOL_COUNT];
    struct lws_http_mount mount;
    struct lws_context_creation_info info;
    struct lws_context * context;
    char * protocol_name;
    char * document_root;
    char * cert_path;
    char * key_path;
    int port;
};

static struct lws_context * jrpc_server_create_context(
    struct jrpc_server * server)
{
    lws_set_log_level(JRPC_DISABLE_LWS_LOG, NULL);

    memset(server->ws_protocols, 0, sizeof(struct lws_protocols) * JRPC_SERVER_PROTOCOL_COUNT);
    server->ws_protocols[0].name = "http";
    server->ws_protocols[0].callback = lws_callback_http_dummy;
    server->ws_protocols[1].name = server->protocol_name;
    jrpc_protocol_init_lws(&server->protocol, &server->ws_protocols[1]);

    memset(&server->mount, 0, sizeof(struct lws_http_mount));
    server->mount.mount_next = NULL;
    server->mount.mountpoint = "/";
    server->mount.origin = server->document_root;
    server->mount.def = "index.html";
    server->mount.origin_protocol = LWSMPRO_FILE;
    server->mount.mountpoint_len = 1;

    memset(&server->info, 0, sizeof(struct lws_context_creation_info));
    server->info.port = server->port;
    server->info.mounts = &server->mount;
    server->info.protocols = server->ws_protocols;
    server->info.vhost_name = "localhost";
    server->info.ws_ping_pong_interval = 10;
    server->info.options = LWS_SERVER_OPTION_HTTP_HEADERS_SECURITY_BEST_PRACTICES_ENFORCE;

    if (NULL == server->document_root)
    {
        // disable http
        server->info.protocols = &server->ws_protocols[1];
        server->info.mounts = NULL;
    }

    if ((NULL != server->cert_path) && (NULL != server->key_path))
    {
        server->info.options |= LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
        server->info.ssl_cert_filepath = server->cert_path;
        server->info.ssl_private_key_filepath = server->key_path;
    }

    struct lws_context * context = lws_create_context(&server->info);
    return context;
}


struct jrpc_server * jrpc_server_create(void)
{
    struct jrpc_server * server = malloc(sizeof(struct jrpc_server));
    if (NULL != server)
    {
        jrpc_server_protocol_init(&server->protocol, server);
        server->protocol_name = strdup(JRPC_SERVER_DEFAULT_PROTOCOL_NAME);
        server->document_root = NULL;
        server->cert_path = NULL;
        server->key_path = NULL;
        server->port = JRPC_SERVER_DEFAULT_PORT;
        server->context = NULL;
    }

    return server;
}

void jrpc_server_dispose(
    struct jrpc_server * server)
{
    if (NULL != server->context)
    {
        lws_context_destroy(server->context);
    }

    jrpc_protocol_cleanup(&server->protocol);
    free(server->protocol_name);
    free(server->document_root);
    free(server->cert_path);
    free(server->key_path);
    free(server);
}

void jrpc_server_set_port(
    struct jrpc_server * server,
    int port)
{
    server->port = port;
}

void jrpc_server_set_onmethod(
    struct jrpc_server * server,
    jrpc_invoke_fn * handler)
{
    server->protocol.onmethod = handler;
}

void jrpc_server_set_onnotify(
    struct jrpc_server * server,
    jrpc_notify_fn * handler)
{
    server->protocol.onnotify = handler;
}

void jrpc_server_set_onconnected(
    struct jrpc_server * server,
    jrpc_connected_fn * handler)
{
    server->protocol.onconnected = handler;
}

void jrpc_server_set_ondisconnected(
    struct jrpc_server * server,
    jrpc_disconnected_fn * handler)
{
    server->protocol.ondisconnected = handler;
}

void jrpc_server_set_protocolname(
    struct jrpc_server * server,
    char const * protocol_name)
{
    free(server->protocol_name);
    server->protocol_name = strdup(protocol_name);
}

void jrpc_server_set_documentroot(
    struct jrpc_server * server,
    char const * document_root)
{
    free(server->document_root);
    server->document_root = strdup(document_root);
}

void jrpc_server_set_certpath(
    struct jrpc_server * server,
    char const * cert_path)
{
    free(server->cert_path);
    server->cert_path = strdup(cert_path);
}

void jrpc_server_set_keypath(
    struct jrpc_server * server,
    char const * key_path)
{
    free(server->key_path);
    server->key_path = strdup(key_path);
}

void jrpc_server_set_userdata(
    struct jrpc_server * server,
    void * user_data)
{
    server->protocol.user_data = user_data;
}

void * jrpc_server_get_userdata(
    struct jrpc_server * server)
{
    return server->protocol.user_data;
}

void jrpc_server_run(
    struct jrpc_server * server,
    int timeout_ms)
{
    if (NULL == server->context)
    {
        server->context = jrpc_server_create_context(server);
    }

    if (NULL != server->context)
    {
        lws_service(server->context, timeout_ms);
    }
}

void jrpc_server_wakeup(
    struct jrpc_server * server)
{
    jrpc_protocol_wakeup(&server->protocol);
}
