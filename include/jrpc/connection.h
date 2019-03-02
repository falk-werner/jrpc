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

/// \brief Sends a response to a connection.
///
/// \note A response can be sent synchrounously within
///       the method invokation or asynchronoulsy at any
///       time later.
///
/// \note Each request is expected to have exactly one
///       response, that can be either a successful result
///       on an error message.
///
/// \param connection Connection that will receive the response
/// \param result any JSON type, representing the response
/// \param id ID of the corresponding request
///
/// \see jrpc_invoke_fn
/// \see jrpc_respond_error
extern JRPC_API void jrpc_respond(
    struct jrpc_connection * connection,
    json_t * result,
    int id);

/// \brief Sends an error message to a connection.
///
/// \note A response can be sent synchrounously within
///       the method invokation or asynchronoulsy at any
///       time later.
///
/// \note Each request is expected to have exactly one
///       response, that can be either a successful result
///       on an error message.
///
/// \param connection Connection that will receive the response
/// \param error_code User defined error code
/// \param error_message User definied error message
/// \param id ID of the corresponding request
///
/// \see jrpc_invoke_fn
/// \see jrpc_respond
extern JRPC_API void jrpc_respond_error(
    struct jrpc_connection * connection,
    int error_code,
    char const * error_message,
    int id);

/// \brief Notfies a given connection.
///
/// Notifications are used as server push mechanism.
///
/// \param connection Connection that will receive the response
/// \param method Name of the notification
/// \param params JSON-array or JSON-object containing the arguments of the notification
extern JRPC_API void jrpc_notify(
    struct jrpc_connection * connection,
    char const * method,
    json_t * params);

/// \brief Returns the server instance associated with the connection.
///
/// \param connection Instance of the connection
/// \return Server instance associated with the connection
extern JRPC_API struct jrpc_server * jrpc_connection_get_server(
    struct jrpc_connection * connection);

/// \brief Sets user specific data for the connection.
///
/// User specific data is not used by JRPC but it might be
/// used by the user within callbacks.
///
/// \note The user is responsible to maintain user specific data,
///       especially to release resources before the connection
///       is released. 
///
/// \param connection Instance of the connection
/// \param user_data User specific data
///
/// \see jrpc_connection_get_userdata
/// \see jrpc_disconnected_fn
extern JRPC_API void jrpc_connection_set_userdata(
    struct jrpc_connection * connection,
    void * user_data);

/// \brief Gets user specific data from the connection.
///
/// User specific data is not used by JRPC but it might be
/// used by the user within callbacks.
///
/// \note The user is responsible to maintain user specific data,
///       especially to release resources before the connection
///       is released. 
///
/// \param connection Instance of the connection
/// \return User specific data
///
/// \see jrpc_connection_set_userdata
/// \see jrpc_disconnected_fn
extern JRPC_API void * jrpc_connection_get_userdata(
    struct jrpc_connection * connection);


#ifdef __cplusplus
}
#endif


#endif
