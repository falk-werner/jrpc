#ifndef JRPC_CONNECTION_INTERN_H
#define JRPC_CONNECTION_INTERN_H

#include "jrpc/connection.h"
#include "jrpc/queue.h"
#include <libwebsockets.h>

struct jrpc_server;

struct jrpc_connection
{
    struct jrpc_server * server;
    struct lws * wsi;
    struct jrpc_queue messages;
    void * user_data;
};

#ifdef __cplusplus
extern "C"
{
#endif

extern void jrpc_connection_init(
    struct jrpc_connection * connection,
    struct jrpc_server * server,
    struct lws * wsi
);

extern void jrpc_connection_cleanup(
    struct jrpc_connection * connection);

#ifdef __cplusplus
}
#endif

#endif
