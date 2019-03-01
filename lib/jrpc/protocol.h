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

#ifndef JRPC_PROTOCOL_H
#define JRPC_PROTOCOL_H

#include "jrpc/server.h"
#include <libwebsockets.h>

struct jrpc_server;

struct jrpc_protocol
{
    struct jrpc_server * server;
    jrpc_invoke_fn * onmethod;
    jrpc_notify_fn * onnotify;
    jrpc_connected_fn * onconnected;
    jrpc_disconnected_fn * ondisconnected;
    void * user_data;
};

#ifdef __cplusplus
extern "C"
{
#endif

extern void jrpc_server_protocol_init(
    struct jrpc_protocol * protocol,
    struct jrpc_server * server);

extern void jrpc_protocol_cleanup(
    struct jrpc_protocol * protocol);

extern void jrpc_protocol_init_lws(
    struct jrpc_protocol * protocol,
    struct lws_protocols * lws_protocol
);

#ifdef __cplusplus
}
#endif


#endif
