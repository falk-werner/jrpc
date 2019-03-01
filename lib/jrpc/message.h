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

#ifndef JRPC_MESSAGE_H
#define JRPC_MESSAGE_H

#include <jansson.h>

#ifndef __cplusplus
#include <stddef.h>
#else
#include <cstddef>
using ::std::size_t;
#endif

struct jrpc_message
{
    struct jrpc_message * next;
    char * data;
    size_t length;
};

#ifdef __cplusplus
extern "C"
{
#endif

extern struct jrpc_message * jrpc_message_create(
    json_t * value);

extern void jrpc_message_dispose(
    struct jrpc_message * message);

#ifdef __cplusplus
}
#endif


#endif
