#include <stdio.h>

#include "context.h"

int send(void * self, local_id dst, const Message * msg) {
    ContextPtr ctx = (ContextPtr) self;
    int ret = ipcContextSend(ctx->ipc, ctx->id, dst, msg);
    if (ret >= 0) {
        printf("%d: Sended %d success to %d\n", ctx->id, msg->s_header.s_type, dst);
        return 0;
    } else {
        printf("%d: Sended %d error to %d\n", ctx->id, msg->s_header.s_type, dst);
        return -1;
    }
}

int send_multicast(void * self, const Message * msg) {
    ContextPtr ctx = (ContextPtr) self;
    for (int id = 0; id < ctx->ipc->proc_cnt; ++id) {
        printf("%d: Sending to %d (total = %d)\n", ctx->id, id, ctx->ipc->proc_cnt);
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
        for (int id = 1; id < ctx->ipc->proc_cnt; ++id) {
            if (ctx->id == id)
                continue;
            // printf("%d: Receiving from %d\n", ctx->id, id);
            if (receive(self, id, msg) == 0)
                return 0;
        }
    }
}
