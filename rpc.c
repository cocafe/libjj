#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include <sys/socket.h>
#include <netdb.h>

#include "rpc.h"

static const char *http_errcode_get(long code)
{
        switch(code) {
        case   0: return "No Response";
        case 101: return "Switching Protocols";
        case 200: return "OK";
        case 201: return "Created";
        case 202: return "Accepted";
        case 203: return "Non-Authoritative Information";
        case 204: return "No Content";
        case 205: return "Reset Content";
        case 206: return "Partial Content";
        case 300: return "Multiple Choices";
        case 301: return "Moved Permanently";
        case 302: return "Found";
        case 303: return "See Other";
        case 304: return "Not Modified";
        case 305: return "Use Proxy";
        case 306: return "(Unused)";
        case 307: return "Temporary Redirect";
        case 400: return "Bad Request";
        case 401: return "Unauthorized";
        case 402: return "Payment Required";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 406: return "Not Acceptable";
        case 407: return "Proxy Authentication Required";
        case 408: return "Request Timeout";
        case 409: return "Conflict";
        case 410: return "Gone";
        case 411: return "Length Required";
        case 412: return "Precondition Failed";
        case 413: return "Request Entity Too Large";
        case 414: return "Request-URI Too Long";
        case 415: return "Unsupported Media Type";
        case 416: return "Requested Range Not Satisfiable";
        case 417: return "Expectation Failed";
        case 500: return "Internal Server Error";
        case 501: return "Not Implemented";
        case 502: return "Bad Gateway";
        case 503: return "Service Unavailable";
        case 504: return "Gateway Timeout";
        case 505: return "HTTP Version Not Supported";
        default:  return "Unknown Error";
        }
}

void http_simple_reason_send(struct evhttp_request *req, int code, char const *text)
{
        char const *code_str = http_errcode_get(code);
        struct evbuffer *body = evbuffer_new();

        evbuffer_add_printf(body, "<h1>%d: %s</h1>", code, code_str);

        if (text) {
                evbuffer_add_printf(body, "%s", text);
        }

        evhttp_send_reply(req, code, code_str, body);

        evbuffer_free(body);
}

void rpc_run(struct rpc_ctx *ctx)
{
        event_base_dispatch(ctx->ev_base);
}

void rpc_stop(struct rpc_ctx *ctx)
{
        event_base_loopbreak(ctx->ev_base);
}

int rpc_init(struct rpc_ctx *ctx,
             const char *bind_addr, uint32_t port,
             void (*cb)(struct evhttp_request *req, void *arg), void *cb_arg)
{
        int err = -EINVAL;

        evthread_use_pthreads();

        ctx->ev_base = event_base_new();
        if (!ctx->ev_base)
                return -EFAULT;

        ctx->ev_http = evhttp_new(ctx->ev_base);
        if (!ctx->ev_http) {
                err = -ENOENT;
                goto free_base;
        }

        evhttp_set_gencb(ctx->ev_http, cb, cb_arg);

        ctx->ev_httpsk = evhttp_bind_socket_with_handle(ctx->ev_http, bind_addr, port);
        if (!ctx->ev_httpsk) {
                err = -EADDRNOTAVAIL;
                goto free_http;
        }

        return 0;

free_http:
        evhttp_free(ctx->ev_http);

free_base:
        event_base_free(ctx->ev_base);

        return err;
}

void rpc_deinit(struct rpc_ctx *ctx)
{
        if (ctx->ev_httpsk)
                evhttp_del_accept_socket(ctx->ev_http, ctx->ev_httpsk);

        if (ctx->ev_http)
                evhttp_free(ctx->ev_http);

        if (ctx->ev_base)
                event_base_free(ctx->ev_base);
}
