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

#ifndef JRPC_SERVER_H
#define JRPC_SERVER_H

#include <jrpc/api.h>
#include <jansson.h>

struct jrpc_server;
struct jrpc_connection;

/// \brief Callback function to invoke a method.
///
/// This callback is used as method handler. It will be called, whenever a connection invokes a method.
/// Each method is expected to be answered, either by a response or by an error.
/// Both, responses or errors may be reported synchrously within the callback or
/// asynchronously at any time later.
///
/// \note Since JRPC server is stateless, it will not keep track on method calls.
/// \note There is also no timeout handling applied by JRPC server.
///
/// \param connection Connection, that invokes the method
/// \param method_name Name of the method to invoke
/// \param params JSON-array or JSON-object containing the arguments of the method
/// \param id ID of the method call, which must be used in response
///
/// \see jrpc_server_set_onmethod
/// \see jrpc_respond
/// \see jrpc_respond_error
typedef void jrpc_invoke_fn(
    struct jrpc_connection * connection,
    char const * method_name,
    json_t * params,
    int id);

/// \brief Callback function to notfiy the server by a connection.
///
/// JRPC server distigueshes between method invokation, where a response or error is
/// expected and notifications, where no response is expected at all.
/// Therefore, notifications do not contain an ID.
///
/// \param connection  Connection, that triggers the notification
/// \param method_name Name of the notification
/// \param params JSON-array or JSON-object containing the arguments of the notification
///
/// \see jrpc_server_set_onnotify
typedef void jrpc_notify_fn(
    struct jrpc_connection * connection,
    char const * method_name,
    json_t * params);

/// \brief Callback function to inform the server about a new connection.
///
/// The callback will be invoked, whenever a new connection is established.
///
/// \param connection Newly established connection.
///
/// \see jrpc_server_set_onconnected
typedef void jrpc_connected_fn(
    struct jrpc_connection * connection);

/// \brief Callback function to inform the server about a closed connection.
///
/// The callback will be invoked, whenever a connection is closed.
///
/// \note All resources associated with the connection will be released
///       immediatly after this callback returns, so it should be used
///       to release any user data that are set for the connection.
///
/// \param connection Already closed connection.
///
/// \see jrpc_server_set_ondisconnected
typedef void jrpc_disconnected_fn(
    struct jrpc_connection * connection);

#ifdef __cplusplus
extern "C"
{
#endif

/// \brief Creates a new instance of a JRPC server.
///
/// \return New instance of JRPC server of NULL, on failure.
extern JRPC_API struct jrpc_server * jrpc_server_create(void);

/// \brief Disposes an instance of a JRPC server.
///
/// Releases all resources associated with the server.
///
/// \param server Instance of the server to dispose
extern JRPC_API void jrpc_server_dispose(
    struct jrpc_server * server);

/// \brief Sets the port of the JRPC server.
///
/// If not set, port 8080 will be used as default.
///
/// \param server Instance of the server
/// \param port new value of server's port
extern JRPC_API void jrpc_server_set_port(
    struct jrpc_server * server,
    int port);

/// \brief Sets the method handler.
///
/// The method handler will be called, whenever a connection invokes a method.
///
/// \note If not set, a default handler will be used, that returns a generic error
///       to the connection for each method invokation.
///
/// \param server Instance of the server
/// \param handler Method handler
///
/// \see jrpc_invoke_fn
extern JRPC_API void jrpc_server_set_onmethod(
    struct jrpc_server * server,
    jrpc_invoke_fn * handler);

/// \brief Sets the notification handler.
///
/// The notification handler will be called, whenever a connection notifies the server.
///
/// \note If not set, a default handler will be used, which swallows silently each 
///       notfication.
///
/// \param server Instance of the server
/// \param handler Notification handler
///
/// \see jrpc_notify_fn
extern JRPC_API void jrpc_server_set_onnotify(
    struct jrpc_server * server,
    jrpc_notify_fn * handler);

/// \brief Sets the connection handler.
///
/// The connection handler will be called, whenever a new connection is
/// established with the server.
///
/// \note There is no way for the server to refuse a connection yet.
///
/// \param server Instance of the server
/// \param handler Connection handler
///
/// \see jrpc_connected_fn
extern JRPC_API void jrpc_server_set_onconnected(
    struct jrpc_server * server,
    jrpc_connected_fn * handler);

/// \brief Sets the handler for closed connections.
///
/// The handler will be called, after a connection is closed.
///
/// \note The handler should be used to release any user specific
///       data associated to the connection, since the connection
///       will be released immediately after the handler returns.
///
/// \param server Instance of the server
/// \param Handler for closed connections
extern JRPC_API void jrpc_server_set_ondisconnected(
    struct jrpc_server * server,
    jrpc_disconnected_fn * handler);

/// \brief Sets the websocket protocol name.
///
/// \note If not specified, "jrpc" will be used as default 
///       protocol name.
///
/// \param server Instance of the server
/// \param protocol_name Name of the websocket protocl
extern JRPC_API void jrpc_server_set_protocolname(
    struct jrpc_server * server,
    char const * protocol_name);

/// \brief Sets the document root to enable HTTP(S) support.
///
/// Document root contains the path to a directory, that will be
/// exposed via HTTP(S) by JRPC server. The HTTP(S) support is
/// intended for demo purposes only, so it's capabilities are
/// very limited, e.g. a file named "index.html" is statically
/// mapped to the default context.
///
/// \note HTTP(S) will be disabled, if no document root is set.
///
/// \param server Instance of the server
/// \param document_root Path to HTTP(S) document root
extern JRPC_API void jrpc_server_set_documentroot(
    struct jrpc_server * server,
    char const * document_root);

/// \brief Sets the path to servers own certificate file.
///
/// \note TLS support will only be activated, if both is set,
///       certificate path and private key path.
///
/// \param server Instance of the server
/// \param cert_path Path to servers own certificate file
///
/// \see jrpc_server_set_keypath
extern JRPC_API void jrpc_server_set_certpath(
    struct jrpc_server * server,
    char const * cert_path);

/// \brief Sets the path to servers private key file.
///
/// \note TLS support will only be activated, if both is set,
///       certificate path and private key path.
///
/// \param server Instance of the server
/// \param key_path Path to servers private key file
///
/// \see jrpc_server_set_certpath
extern JRPC_API void jrpc_server_set_keypath(
    struct jrpc_server * server,
    char const * key_path);

/// \brief Sets user specific data.
///
/// User specific data is not used by the server itself, but
/// it can be used by the user within callbacks.
///
/// \note User specific data is not maintained by the server,
///       so the user is responsible to release any associated
///       resources, before the server instance is released.
///
/// \param server Instance of the server
/// \param user_data User specific data
///
/// \see jrpc_server_get_userdata
extern JRPC_API void jrpc_server_set_userdata(
    struct jrpc_server * server,
    void * user_data);

/// \brief Retrieved previously set user specific data.
///
/// User specific data is not used by the server itself, but
/// it can be used by the user within callbacks.
///
/// \note User specific data is not maintained by the server,
///       so the user is responsible to release any associated
///       resources, before the server instance is released.
///
/// \param server Instance of the server
/// \return User specific data
///
/// \see jrpc_server_set_userdata
extern JRPC_API void * jrpc_server_get_userdata(
    struct jrpc_server * server);

/// \brief Runs the server until some event occurs or timeout.
///
/// \note All configuration must be done before the first call
///       of this function.
///
/// \note This function blocks the caller
///
/// \param server Instance of the server
/// \param timeout_ms Milliseconds of timeout, use 0 to poll without any blocking
///
/// \see jrpc_server_wakeup
extern JRPC_API void jrpc_server_run(
    struct jrpc_server * server,
    int timeout_ms);

/// \brief Wakeup JRPC server.
///
/// This function is used to safely interrupt jrpc_server_run from
/// another thread.
///
/// \not This is the only function that can be called safely from
///      a foreign thread context. All other functions must be
///      called from the thread, which is running JRPC server.
///
/// \params server Instance of the server
///
/// \see jrpc_server_run
extern JRPC_API void jrpc_server_wakeup(
    struct jrpc_server * server);

#ifdef __cplusplus
}
#endif

#endif
