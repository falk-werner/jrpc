#ifndef JRPC_CONNECTION_H
#define JRPC_CONNECTION_H

#include <jrpc/api.h>
#include <jansson.h>

struct jrpc_connection;
struct jrpc_server;

#ifdef __cplusplus
extern "C"
{
#endif

extern JRPC_API void jrpc_respond(
    struct jrpc_connection * connection,
    json_t * result,
    int id);

extern JRPC_API void jrpc_respond_error(
    struct jrpc_connection * connection,
    int error_code,
    char const * error_message,
    int id);

extern JRPC_API void jrpc_notify(
    struct jrpc_connection * connection,
    json_t * content);

extern JRPC_API void jrpc_notify_all(
    struct jrpc_connection * connection,
    json_t * content);

extern JRPC_API struct jrpc_server * jrpc_connection_get_server(
    struct jrpc_connection * connection);

extern JRPC_API void jrpc_connection_set_userdata(
    struct jrpc_connection * connection,
    void * user_data);

extern JRPC_API void * jrpc_connection_get_userdata(
    struct jrpc_connection * connection);


#ifdef __cplusplus
}
#endif


#endif
