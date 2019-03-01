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

#include "jrpc/message.h"

#include <libwebsockets.h>
#include <stdlib.h>

static size_t jrpc_message_measure(
    json_t * value)
{
    return json_dumpb(value, NULL, 0, JSON_COMPACT);
}

static void jrpc_message_serialize(
    struct jrpc_message * message,
    json_t * value)
{
    json_dumpb(value, message->data, message->length, JSON_COMPACT);
}

struct jrpc_message * jrpc_message_create(
    json_t * value)
{
    size_t message_size = jrpc_message_measure(value);
    char * data = malloc(sizeof(struct jrpc_message) + LWS_PRE + message_size);
    struct jrpc_message * message = (struct jrpc_message * ) data;
    if (NULL != message)
    {
        message->data = &data[sizeof(struct jrpc_message) + LWS_PRE];
        message->length = message_size;
        message->next = NULL;

        jrpc_message_serialize(message, value);
    }

    return message;
}

void jrpc_message_dispose(
    struct jrpc_message * message)
{
    free(message);
}
