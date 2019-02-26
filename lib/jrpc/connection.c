#include "jrpc/connection.h"

#include <stddef.h>

void jrpc_respond(
    struct jrpc_connection * connection,
    json_t * result,
    int id)
{

}

void jrpc_respond_error(
    struct jrpc_connection * connection,
    int error_code,
    char const * error_message,
    int id)
{

}

void jrpc_notify(
    struct jrpc_connection * connection,
    json_t * content)
{

}

void jrpc_notify_all(
    struct jrpc_connection * connection,
    json_t * content)
{

}

struct jrpc_server * jrpc_connection_get_server(
    struct jrpc_connection * connection)
{
    return NULL;
}

void jrpc_connection_set_userdata(
    struct jrpc_connection * connection,
    void * user_data)
{
}

void * jrpc_connection_get_userdata(
    struct jrpc_connection * connection)
{
    return NULL;
}
