cmake_minimum_required (VERSION 3.1)
project(jrpc VERSION 1.0.0 DESCRIPTION "Yet another JSON-RPC server based on libwebsockets")

option(WITHOUT_EXAMPLE "disable example" OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LWS REQUIRED libwebsockets)
pkg_check_modules(JANSSON REQUIRED jansson)

set(CMAKE_C_STANDARD 99)
set(C_WARNINGS -Wall -Wextra)

add_library(jrpc STATIC
    lib/jrpc/server.c
    lib/jrpc/connection.c
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
    example/chat-server/main.c
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