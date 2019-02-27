#include <jrpc.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>

#include <unistd.h>
#include <getopt.h>

static struct jrpc_server * server;

static void onmethod(
    struct jrpc_connection * connection,
    char const * method_name,
    json_t * params,
    int id)
{
    puts("method called");

    jrpc_respond_error(connection, 42, "not implemented", id);
}


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

static void on_shutdown_requested(int signal_id)
{
    jrpc_server_shutdown(server);
}

static void printUsage(void)
{
    printf(
        "chat-server, Copyright (C) 2019, Falk Werner\n"
        "Example for jrpc library\n"
        "\n"
        "Usage: chat-server [-p <port>] [-d <document_root>]\n"
        "                   [-c <cert_path>] [-k <key_path]\n"
        "\n"
        "Options:\n"
        "\t-p, --port          Number of servers port (default: 8080)\n"
        "\t-d, --document_root Path of ww directory (default: not set, disabled)\n"
        "\t-c, --cert_path     Path of servers own certifcate (default: not set, TLS disabled)\n"
        "\t-k, --key_path      Path of servers private key (default: not set, TLS disabled)\n"
        "\n"
    );
}

static int parse_arguments(
    int argc,
    char* argv[],
    struct jrpc_server * server)
{
    static struct option const options[] =
    {
        {"document_root", required_argument, NULL, 'd'},
        {"cert_path", required_argument, NULL, 'c'},
        {"key_path", required_argument, NULL, 'k'},
        {"port", required_argument, NULL, 'p'},
        {"help", required_argument, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };

    int result = EXIT_SUCCESS;
    bool is_finished = false;
    bool show_help = false;
    while (!is_finished)
    {
        int option_index = 0;
        int const c = getopt_long(argc, argv, "d:c:k:p:h", options, &option_index);

        switch (c)
        {
            case -1:
                is_finished = true;
                break;
            case 'h':
                show_help = true;
                is_finished = true;
                break;
            case 'd':
                jrpc_server_set_documentroot(server, optarg);
                break;
            case 'c':
                jrpc_server_set_certpath(server, optarg);
                break;
            case 'k':
                jrpc_server_set_keypath(server, optarg);
                break;
            case 'p':
                jrpc_server_set_port(server, atoi(optarg));
                break;
            default:
                fprintf(stderr, "error: unknown argument\n");
                is_finished = true;
                result = EXIT_FAILURE;
                break;
        }
    }

    if (show_help)
    {
        printUsage();
    }

    return result;
}

int main(int argc, char * argv[])
{
    server = jrpc_server_create();
    jrpc_server_set_onconnected(server, &onconnected);
    jrpc_server_set_ondisconnected(server, &ondisconnected);
    jrpc_server_set_onmethod(server, &onmethod);

    int result = parse_arguments(argc, argv, server);
    if (EXIT_SUCCESS == result)
    {
        signal(SIGINT, &on_shutdown_requested);
        jrpc_server_run(server);
    }

    jrpc_server_dispose(server);
    return result;
}