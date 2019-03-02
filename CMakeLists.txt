# JRPC - Yet another JSON-RPC server based on libwebsockets
#  <https://github.com/falk-werner/jrpc>
#
# Copyright (c) 2019 Falk Werner
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required (VERSION 3.1)
project(jrpc VERSION 1.1.0 DESCRIPTION "Yet another JSON-RPC server based on libwebsockets")

option(WITHOUT_EXAMPLE "disable example" OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LWS REQUIRED libwebsockets)
pkg_check_modules(JANSSON REQUIRED jansson)

set(CMAKE_C_STANDARD 99)
set(C_WARNINGS -Wall -Wextra)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(jrpc STATIC
    lib/jrpc/message.c
    lib/jrpc/queue.c
    lib/jrpc/server.c
    lib/jrpc/connection.c
    lib/jrpc/protocol.c
)

target_include_directories(jrpc PUBLIC
    include
    lib
    ${LWS_INCLUDE_DIRS}
    ${JANSSON_INCLUDE_DIRS}
)

target_compile_options(jrpc PUBLIC
    ${CMAKE_C_FLAGS}
    ${C_WARNINGS}
    ${LWS_CFLAGS_OTHER}
    ${JANSSON_CFLAGS_OTHER}
)

file(WRITE "${PROJECT_BINARY_DIR}/libjrpc.pc"
"prefix=\"${CMAKE_INSTALL_PREFIX}\"

exec_prefix=\${prefix}
libdir=\${exec_prefix}/lib${LIB_SUFFIX}
includedir=\${prefix}/include
Name: libwsfs
Description: Yet another JSON-RPC server based on libwebsockets
Version: ${PROJECT_VERSION}

Libs: -L\${libdir} -ljrpc -l${LWS_LIBRARIES} -l${JANSSON_LIBRARIES}
Cflags: -I\${includedir}"
)

install(TARGETS jrpc DESTINATION lib${LIB_SUFFIX})
install(DIRECTORY include DESTINATION include)
install(FILES "${PROJECT_BINARY_DIR}/libjrpc.pc" DESTINATION lib${LIB_SUFFIX}/pkgconfig)

if(NOT WITHOUT_EXAMPLE)

add_executable(chat-server
    example/chat-server/main.cc
)


target_include_directories(chat-server PUBLIC
    include
    ${LWS_INCLUDE_DIRS}
    ${JANSSON_INCLUDE_DIRS}
)

target_compile_options(chat-server PUBLIC
    ${CMAKE_C_FLAGS}
    ${C_WARNINGS}
    ${LWS_CFLAGS_OTHER}
    ${JANSSON_CFLAGS_OTHER}
)

target_link_libraries(chat-server PUBLIC
    jrpc
    ${LWS_LIBRARIES}
    ${JANSSON_LIBRARIES}
)


endif(NOT WITHOUT_EXAMPLE)