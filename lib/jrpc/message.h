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
