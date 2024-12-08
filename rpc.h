#ifndef __LIBJJ_RPC_H__
#define __LIBJJ_RPC_H__

#include <event2/event.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <event2/buffer.h>
#include <event2/buffer_compat.h>
#include <event2/thread.h>

struct rpc_ctx {
        struct event_base *ev_base;
        struct evhttp *ev_http;
        struct evhttp_bound_socket *ev_httpsk;
};

void http_simple_reason_send(struct evhttp_request *req, int code, char const *text);

void rpc_run(struct rpc_ctx *ctx);
void rpc_stop(struct rpc_ctx *ctx);

int rpc_init(struct rpc_ctx *ctx,
             const char *bind_addr, uint32_t port,
             void (*cb)(struct evhttp_request *req, void *arg), void *cb_arg);
void rpc_deinit(struct rpc_ctx *ctx);

#endif // __LIBJJ_RPC_H__