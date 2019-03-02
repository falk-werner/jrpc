[![Build Status](https://travis-ci.org/falk-werner/jrpc.svg?branch=master)](https://travis-ci.org/falk-werner/jrpc)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/1eb7cdd821704b7a9b5dee83c32ab541)](https://www.codacy.com/app/falk.werner/jrpc?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=falk-werner/jrpc&amp;utm_campaign=Badge_Grade)

# jrpc

Yet another JSON-RPC server based on libwebsockets.

## Features

JRPC provides a library to create [JSON-RPC](https://www.jsonrpc.org/specification_v1) server applications based on libsockets.
In contrast to pure HTTP-based solutions, JRPC allows also to send notifications from server to 
connected clients (aka. server push) thanks to websockets protocol.

-   invoke methods on server
-   notify server
-   notify clients (server push)
-   synchronous and asynchronous responses
-   stateless
-   single threaded

## Communication

JRPC supports three kinds of messages:

-   **request** to invoke a method
-   **response** to answer a method
-   **notification** to send information without expecting a response

### Request

    client: {"method": "add", "params": [1, 1], "id": 42}
    server: {"result": 3, "id": 42}

A request is always sent by the client and is used to invoke a method on the server. For each request, exactly one response is expected.

**Note:** JRPC does not keep track on method calls. The user code is fully responsible to answer a request. Note also, that there is no timeout handling done by JRPC.

| Item        | Data type       | Description                       |
| ----------- |:---------------:| --------------------------------- |
| method      | string          | name of the method to invoke      |
| params      | array or object | method specific parameters        |
| id          | integer         | id, which is repeated in response |

### Response

    # successful response
    client: {"method": "add", "params": [1, 1], "id": 42}
    server: {"result": 3, "id": 42}

    # error
    client: {"method": "div", "params": [1, 0], "id": 42}
    server: {"error": {"code": 1337, "message": "div by zero"}, "id": 42}

A response is alwasy sent by server to client as result of a method invokation. There are two kinds of responses:

-   successful results  
    contains a field *result*, containing the results of the method invokation

-   error messages  
    contains a fiels *error*, containing detailed description of the error

| Item          | Data type | Description                                                           |
| ------------- |:---------:| --------------------------------------------------------------------- |
| result        | any       | successful result only; contains the results of the method invokation |
| error         | object    | errors only; contains information about the error                     |
| error.code    | integer   | errors only; contains a user specified code of the error              |
| error.message | string    | errors only; contains a user specified code of the error              |
| id            | integer   | id of the corresponding request                                       |

### Notfication

    # client to server
    client: {"method": "postMessage", "params": ["@Bob: bye ;-)"]}
    
    # server to server
    server: {"method": "userLeft", "params": ["Bob"]}

Notifications are the only type of message that can be sent in both directions, from client to server and from server to client. Technically, they are similar to requests, but without an ID.

| Item        | Data type       | Description                       |
| ----------- |:---------------:| --------------------------------- |
| method      | string          | name of the method to invoke      |
| params      | array or object | method specific parameters        |

## Build and run

To install dependencies, see below.

    cd jrpc
    mkdir .build
    cd .build
    cmake ..
    make
    ./chat-server -d ../example/chat-server/www/

### Build options

By default, build of example application is enabled. You can disable it using the following cmake option:

-   **WITHOUT_EXAMPLE**: disable example
    `cmake -DWITHOUT_EXAMPLE=ON ..`

## Dependencies

-   [libwebsockets](https://libwebsockets.org/)
-   [Jansson](https://jansson.readthedocs.io)

### libwebsockets

    wget -O libwebsockets-3.1.0.tar.gz https://github.com/warmcat/libwebsockets/archive/v3.1.0.tar.gz
    tar -xf libwebsockets-3.1.0.tar.gz
    cd libwebsockets-3.1.0
    mkdir .build
    cd .build
    cmake ..
    make
    sudo make install

### Jansson

On many systems, libjansson can installed via apt:
    
    sudo apt install libjansson-dev

Otherwise, it can be installed from source:
    
    wget -O libjansson-2.12.tar.gz https://github.com/akheron/jansson/archive/v2.12.tar.gz
    tar -xf libjansson-2.12.tar.gz
    cd jansson-2.12
    mkdir .build
    cd .build
    cmake ..
    make
    sudo make install