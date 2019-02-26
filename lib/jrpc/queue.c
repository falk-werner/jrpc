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
