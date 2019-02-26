#ifndef JRPC_API_H
#define JRPC_API_H

#ifndef JRPC_API
#define JRPC_API
#endif

#ifdef __GNUC__
#define JRPC_EXPORT __attribute__ ((visiblity ("default")))
#else
#define JRPC_EXPORT
#endif

#endif
