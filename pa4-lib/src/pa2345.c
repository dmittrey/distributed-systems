#include <stdio.h>

#include "banking.h"
#include "message.h"
#include "context.h"

extern timestamp_t lamport_time;

timestamp_t get_lamport_time() {
    return lamport_time;
}

int request_cs(const void * self) {
    ContextPtr ctx = (ContextPtr) self;
    queuePush(ctx->queue, lamport_time + 1, ctx->id);

    Message msg;
    CSRequestMessage(&msg);
    return send_multicast(ctx, &msg);
}

int release_cs(const void * self) {
    ContextPtr ctx = (ContextPtr) self;
    queuePop(ctx->queue);

    Message msg;
    CSReleaseMessage(&msg);
    return send_multicast(ctx, &msg);
}
