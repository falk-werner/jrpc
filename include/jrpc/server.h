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

#ifndef JRPC_SERVER_H
#define JRPC_SERVER_H

#include <jrpc/api.h>
#include <jansson.h>

struct jrpc_server;
struct jrpc_connection;

typedef void jrpc_invoke_fn(
    struct jrpc_connection * connection,
    char const * method_name,
    json_t * params,
    int id);

typedef void jrpc_notify_fn(
    struct jrpc_connection * connection,
    char const * method_name,
    json_t * content);

typedef void jrpc_connected_fn(
    struct jrpc_connection * connection);

typedef void jrpc_disconnected_fn(
    struct jrpc_connection * connection);

#ifdef __cplusplus
extern "C"
{
#endif

extern JRPC_API struct jrpc_server * jrpc_server_create(void);

extern JRPC_API void jrpc_server_dispose(
    struct jrpc_server * server);

extern JRPC_API void jrpc_server_set_port(
    struct jrpc_server * server,
    int port);

extern JRPC_API void jrpc_server_set_onmethod(
    struct jrpc_server * server,
    jrpc_invoke_fn * handler);

extern JRPC_API void jrpc_server_set_onnotify(
    struct jrpc_server * server,
    jrpc_notify_fn * handler);

extern JRPC_API void jrpc_server_set_onconnected(
    struct jrpc_server * server,
    jrpc_connected_fn * handler);

extern JRPC_API void jrpc_server_set_ondisconnected(
    struct jrpc_server * server,
    jrpc_disconnected_fn * handler);

extern JRPC_API void jrpc_server_set_protocolname(
    struct jrpc_server * server,
    char const * protocol_name);

extern JRPC_API void jrpc_server_set_documentroot(
    struct jrpc_server * server,
    char const * document_root);

extern JRPC_API void jrpc_server_set_certpath(
    struct jrpc_server * server,
    char const * cert_path);

extern JRPC_API void jrpc_server_set_keypath(
    struct jrpc_server * server,
    char const * key_path);

extern JRPC_API void jrpc_server_set_userdata(
    struct jrpc_server * server,
    void * user_data);

extern JRPC_API void * jrpc_server_get_userdata(
    struct jrpc_server * server);

extern JRPC_API void jrpc_server_run(
    struct jrpc_server * server,
    int timeout_ms);

#ifdef __cplusplus
}
#endif

#endif
