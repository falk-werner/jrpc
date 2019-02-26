#ifndef JRPC_UTIL_H
#define JRPC_UTIL_H

#ifdef __GNUC__
#define JRPC_UNUSED_PARAM(param) param __attribute__((unused))
#else
#define JRPC_UNUSED_PARAM(param)
#endif

#endif