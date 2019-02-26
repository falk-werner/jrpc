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
