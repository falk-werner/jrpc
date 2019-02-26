[![Build Status](https://travis-ci.org/falk-werner/jrpc.svg?branch=master)](https://travis-ci.org/falk-werner/jrpc)

# jrpc

Yet another JSON-RPC server based on libwebsockets.

## Build and run

To install dependencies, see below.

    cd jrpc
    mkdir .build
    cd .build
    cmake ..
    make
    ./chat-server

### Build options

By default, build of example application is enabled. You can disable it using the following cmake option:

-   **WITHOUT_EXAMPLE**: disable example
    `cmake -DWITHOUD_EXAMPLE=ON ..`

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