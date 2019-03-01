#include <jrpc.h>

#include <signal.h>

#include <unistd.h>
#include <getopt.h>

#include <iostream>
#include <cstdlib>

namespace
{

static struct jrpc_server * server;

void onmethod(
    struct jrpc_connection * connection,
    char const * method_name,
    json_t * params,
    int id)
{
    std::cout << "method called" << std::endl;

    jrpc_respond_error(connection, 42, "not implemented", id);
}


void onconnected(
    struct jrpc_connection * connection)
{
    std::cout << "connected" << std::endl;
}

void ondisconnected(
    struct jrpc_connection * connection)
{
    std::cout << "disconnection" << std::endl;
}

void on_shutdown_requested(int signal_id)
{
    jrpc_server_shutdown(server);
}

void printUsage(void)
{
    std::cout
        << "chat-server, Copyright (C) 2019, Falk Werner" << std::endl
        << "Example for jrpc library" << std::endl
        << std::endl
        << "Usage: chat-server [-p <port>] [-d <document_root>]" << std::endl
        << "                   [-c <cert_path>] [-k <key_path]" << std::endl
        << std::endl
        << "Options:" << std::endl
        << "\t-p, --port          Number of servers port (default: 8080)" << std::endl
        << "\t-d, --document_root Path of ww directory (default: not set, disabled)" << std::endl
        << "\t-c, --cert_path     Path of servers own certifcate (default: not set, TLS disabled)" << std::endl
        << "\t-k, --key_path      Path of servers private key (default: not set, TLS disabled)" << std::endl
        << std::endl
    ;
}

int parse_arguments(
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
        {"help", no_argument, NULL, 'h'},
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
        result = EXIT_FAILURE;
    }

    return result;
}

}

int main(int argc, char * argv[])
{
    server = jrpc_server_create();
    jrpc_server_set_protocolname(server, "jrpc-chat");
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