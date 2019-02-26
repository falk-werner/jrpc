#include "jrpc/server.h"
#include <stddef.h>

struct jrpc_server * jrpc_server_create(void)
{
    return NULL;
}

void jrpc_server_dispose(
    struct jrpc_server * server)
{

}

void jrpc_server_set_port(
    struct jrpc_server * server,
    int port)
{

}

void jrpc_server_set_onmethod(
    struct jrpc_server * server,
    jrpc_invoke_fn * handler)
{

}

void jrpc_server_set_onnotify(
    struct jrpc_server * server,
    jrpc_notify_fn * handler)
{

}

void jrpc_server_set_onconnected(
    struct jrpc_server * server,
    jrpc_connected_fn * handler)
{

}

void jrpc_server_set_ondisconnected(
    struct jrpc_server * server,
    jrpc_disconnected_fn * handler)
{

}

void jrpc_server_set_documentroot(
    struct jrpc_server * server,
    char const * document_root)
{

}

void jrpc_server_set_userdata(
    struct jrpc_server * server,
    void * user_data)
{

}

void * jrpc_server_get_userdata(
    struct jrpc_server * server)
{
    return NULL;
}

void jrpc_server_run(
    struct jrpc_server * server)
{

}

void jrpc_server_shutdown(
    struct jrpc_server * server)
{

}