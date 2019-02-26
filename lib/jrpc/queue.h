#ifndef JRPC_QUEUE_H
#define JRPC_QUEUE_H

#ifndef _cplusplus
#include <stdbool.h>
#endif

struct jrpc_message;

struct jrpc_queue
{
    struct jrpc_message * first;
    struct jrpc_message * last;
};

#ifdef __cplusplus
extern "C"
{
#endif

extern void jrpc_queue_init(
    struct jrpc_queue * queue);

extern void jrpc_queue_cleanup(
    struct jrpc_queue * queue);

extern bool jrpc_queue_is_empty(
    struct jrpc_queue * queue);

extern void jrpc_queue_append(
    struct jrpc_queue * queue,
    struct jrpc_message * message);

extern struct jrpc_message * jrpc_queue_dequeue(
    struct jrpc_queue * queue);

#ifdef __cplusplus
}
#endif


#endif
