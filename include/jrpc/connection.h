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

#ifndef JRPC_CONNECTION_H
#define JRPC_CONNECTION_H

#include <jrpc/api.h>
#include <jansson.h>

struct jrpc_connection;
struct jrpc_server;

#ifdef __cplusplus
extern "C"
{
#endif

extern JRPC_API void jrpc_respond(
    struct jrpc_connection * connection,
    json_t * result,
    int id);

extern JRPC_API void jrpc_respond_error(
    struct jrpc_connection * connection,
    int error_code,
    char const * error_message,
    int id);

extern JRPC_API void jrpc_notify(
    struct jrpc_connection * connection,
    char const * method,
    json_t * params);

extern JRPC_API struct jrpc_server * jrpc_connection_get_server(
    struct jrpc_connection * connection);

extern JRPC_API void jrpc_connection_set_userdata(
    struct jrpc_connection * connection,
    void * user_data);

extern JRPC_API void * jrpc_connection_get_userdata(
    struct jrpc_connection * connection);


#ifdef __cplusplus
}
#endif


#endif
