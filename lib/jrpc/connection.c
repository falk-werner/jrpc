#include "jrpc/connection_intern.h"
#include "jrpc/message.h"

#include <stddef.h>

static void jrpc_connection_send(
    struct jrpc_connection * connection,
    json_t * message_data)
{
    struct jrpc_message * message = jrpc_message_create(message_data);
    if (NULL != message)
    {
        jrpc_queue_append(&connection->messages, message);
        lws_callback_on_writable(connection->wsi);
    }

    json_decref(message_data); 

}

void jrpc_connection_init(
    struct jrpc_connection * connection,
    struct jrpc_server * server,
    struct lws * wsi
)
{
    connection->server = server;
    connection->wsi = wsi;
    connection->user_data = NULL;

    jrpc_queue_init(&connection->messages);
}

void jrpc_connection_cleanup(
    struct jrpc_connection * connection)
{
    jrpc_queue_cleanup(&connection->messages);
}

void jrpc_respond(
    struct jrpc_connection * connection,
    json_t * result,
    int id)
{
    json_t * response = json_object();
    json_object_set_new(response, "result", result);
    json_object_set_new(response, "id", json_integer(id));

    jrpc_connection_send(connection, response);
}

void jrpc_respond_error(
    struct jrpc_connection * connection,
    int error_code,
    char const * error_message,
    int id)
{
    json_t * error_holder = json_object();
    json_object_set_new(error_holder, "code", json_integer(error_code));
    json_object_set_new(error_holder, "message", json_string(error_message));

    json_t * response = json_object();
    json_object_set_new(response, "error", error_holder);
    json_object_set_new(response, "id", json_integer(id));

    jrpc_connection_send(connection, response);
}

void jrpc_notify(
    struct jrpc_connection * connection,
    char const * method,
    json_t * params)
{
    json_t * notification = json_object();
    json_object_set_new(notification, "method", json_string(method));
    json_object_set_new(notification, "params", params);

    jrpc_connection_send(connection, notification);

}

struct jrpc_server * jrpc_connection_get_server(
    struct jrpc_connection * connection)
{
    return connection->server;
}

void jrpc_connection_set_userdata(
    struct jrpc_connection * connection,
    void * user_data)
{
    connection->user_data = user_data;
}

void * jrpc_connection_get_userdata(
    struct jrpc_connection * connection)
{
    return connection->user_data;
}
