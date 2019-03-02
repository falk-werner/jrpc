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

#include <iostream>
#include <unordered_map>

#include <cstdlib>
#include <cstring>
#include <csignal>

#include <unistd.h>
#include <getopt.h>

#include <jrpc.h>

namespace
{

struct chatter
{
    jrpc_connection * connection;
    std::string name;    
};

static volatile bool is_shutdown_requested;
static std::unordered_map<jrpc_connection *, chatter> chatters;

bool is_name_used(char const * name)
{
    for(const auto& item: chatters)
    {
        chatter const & actual = item.second;        
        if (0 == actual.name.compare(name))
        {
            return true;
        }
    }

    return false;
}

void announce(
    char const * what,
    char const * who
)
{
    for(auto const & item: chatters) 
    {
        chatter const & actual = item.second;

        json_t * params = json_array();
        json_array_append_new(params, json_string(what));
        json_array_append_new(params, json_string(who));

        jrpc_notify(actual.connection, "announce", params);
    }
}


void set_name(
    chatter & who,
    json_t * params,
    int id)
{
    json_t * name_holder = json_array_get(params, 0);
    if ((nullptr != name_holder) && (json_is_string(name_holder))) 
    {
        char const * name = json_string_value(name_holder);
        if (!is_name_used(name))
        {
            who.name = name;

            json_t * result = json_object();
            json_object_set_new(result, "name", json_string(name));
            jrpc_respond(who.connection, result, id);

            announce("arrived", who.name.c_str());
        }
        else
        {
            jrpc_respond_error(who.connection, 1, "name already used", id);
        }
    }
    else
    {
        jrpc_respond_error(who.connection, -1, "invalid params", id);
    }
    
}

void onmethod(
    jrpc_connection * connection,
    char const * method_name,
    json_t * params,
    int id)
{
    auto it = chatters.find(connection);
    if (it != chatters.end())
    {
        chatter & who = it->second;
        if (0 == strcmp("set_name", method_name))
        {
            set_name(who, params, id);
        }
        else
        {
            jrpc_respond_error(connection, -1, "not implemented", id);
        }
    }
    else
    {
        jrpc_respond_error(connection, -1, "invalid connection", id);
    }
}

void chat(
    chatter & who,
    json_t * params
)
{
    json_t * message_holder = json_array_get(params, 0);
    if ((nullptr != message_holder) && (json_is_string(message_holder))) 
    {
        char const * message = json_string_value(message_holder);

        for(auto const & item: chatters) 
        {
            chatter const & actual = item.second;

            json_t * post = json_array();
            json_array_append_new(post, json_string(who.name.c_str()));
            json_array_append_new(post, json_string(message));

            jrpc_notify(actual.connection, "message", post);
        }
    }
}

void onnotify(
    jrpc_connection * connection,
    char const * method_name,
    json_t * params
)
{
    auto it = chatters.find(connection);
    if (it != chatters.end())
    {
        chatter & who = it->second;

        if (0 == strcmp("chat", method_name))
        {
            chat(who, params);
        }
    }
}

void onconnected(
    jrpc_connection * connection)
{
    chatters.insert({connection, {connection, ""}});
}

void ondisconnected(
    jrpc_connection * connection)
{
    auto it = chatters.find(connection);
    if (it != chatters.end())
    {
        chatter & who = it->second;
        announce("gone", who.name.c_str());

        chatters.erase(connection);
    }
}

void on_shutdown_requested(int signal_id)
{
    (void) signal_id;

    is_shutdown_requested = true;
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
    static option const options[] =
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
    jrpc_server * server = jrpc_server_create();
    jrpc_server_set_protocolname(server, "jrpc-chat");
    jrpc_server_set_onconnected(server, &onconnected);
    jrpc_server_set_ondisconnected(server, &ondisconnected);
    jrpc_server_set_onmethod(server, &onmethod);
    jrpc_server_set_onnotify(server, &onnotify);

    int result = parse_arguments(argc, argv, server);
    if (EXIT_SUCCESS == result)
    {
        is_shutdown_requested = false;
        signal(SIGINT, &on_shutdown_requested);

        while (!is_shutdown_requested)
        {
            int const timeout_ms = 1000;
            jrpc_server_run(server, timeout_ms);
        }
    }

    jrpc_server_dispose(server);
    return result;
}