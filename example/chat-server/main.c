#include <jrpc.h>

#include <stdio.h>
#include <stdlib.h>

static void onconnected(
    struct jrpc_connection * connection)
{
    puts("connected");
}

static void ondisconnected(
    struct jrpc_connection * connection)
{
    puts("disconnection");
}

int main(int argc, char * argv[])
{
    struct jrpc_server * server = jrpc_server_create();

    jrpc_server_set_port(server, 8080);
    jrpc_server_set_onconnected(server, &onconnected);
    jrpc_server_set_ondisconnected(server, &ondisconnected);

    jrpc_server_run(server);
    jrpc_server_dispose(server);

    return EXIT_SUCCESS;
}