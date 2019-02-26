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
