#include <stdio.h>
#include <unistd.h>

#include "context.h"

int send(void * self, local_id dst, const Message * msg) {
    ContextPtr ctx = (ContextPtr) self;
    int ret = ipcContextSend(ctx->ipc, ctx->id, dst, msg);
    if (ret >= 0) {
        return 0;
    } else {
        return -1;
    }
}

int send_multicast(void * self, const Message * msg) {
    ContextPtr ctx = (ContextPtr) self;
    for (int id = 0; id < ctx->ipc->proc_cnt; ++id) {
        if (ctx->id == id)
            continue;
        if (send(self, id, msg) < 0) {
            return -1;
        }
    }
    return 0;
}

int receive(void * self, local_id from, Message * msg) {
    ContextPtr ctx = (ContextPtr) self;
    if (ipcContextReceive(ctx->ipc, from, ctx->id, msg) <= 0)
        return -1;
    return 0;
}

int receive_any(void * self, Message * msg) {
    ContextPtr ctx = (ContextPtr) self;
    while (1) {
        for (int id = 0; id < ctx->ipc->proc_cnt; ++id) {
            if (ctx->id == id)
                continue;
            if (receive(self, id, msg) == 0) {
                return 0;
            }
        }
    }
}
