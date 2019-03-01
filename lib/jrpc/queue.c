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

#include "jrpc/queue.h"
#include "jrpc/message.h"

#include <stddef.h>

void jrpc_queue_init(
    struct jrpc_queue * queue)
{
    queue->first = NULL;
    queue->last = NULL;
}

void jrpc_queue_cleanup(
    struct jrpc_queue * queue)
{
    struct jrpc_message * message = queue->first;
    while (NULL != message)
    {
        struct jrpc_message * next = message->next;
        jrpc_message_dispose(message);
        message = next;
    }

    jrpc_queue_init(queue);
}

bool jrpc_queue_is_empty(
    struct jrpc_queue * queue)
{
    return (NULL == queue->first);
}

void jrpc_queue_append(
    struct jrpc_queue * queue,
    struct jrpc_message * message)
{
    message->next = NULL;

    if (NULL != queue->last)
    {
        queue->last->next = message;
        queue->last = message;
    }
    else
    {
        queue->first = message;
        queue->last = message;
    }
}

struct jrpc_message * jrpc_queue_dequeue(
    struct jrpc_queue * queue)
{
    struct jrpc_message * const result = queue->first;
    if (NULL != result)
    {
        queue->first = queue->first->next;

        if (NULL == queue->first)
        {
            queue->last = NULL;
        }
    }

    return result;
}
