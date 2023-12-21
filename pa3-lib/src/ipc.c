#include <stdio.h>
#include <unistd.h>

#include "context.h"

timestamp_t lamport_time;

int send(void * self, local_id dst, const Message * msg) {
    ContextPtr ctx = (ContextPtr) self;
    lamport_time++;

    Message* unsafe = (Message*)msg;
    unsafe->s_header.s_local_time = lamport_time;
    
    int ret = ipcContextSend(ctx->ipc, ctx->id, dst, msg);
    if (ret >= 0) {
        return 0;
    } else {
        return -1;
    }
}

int send_multicast(void * self, const Message * msg) {
    ContextPtr ctx = (ContextPtr) self;
    for (int id = 0; id < ctx->host_cnt; ++id) {
        if (ctx->id == id)
            continue;
        if (send(self, id, msg) < 0) {
            return -1;
        }
        lamport_time--;
    }
    lamport_time++;
    return 0;
}

int receive(void * self, local_id from, Message * msg) {
    ContextPtr ctx = (ContextPtr) self;
    if (ipcContextReceive(ctx->ipc, from, ctx->id, msg) <= 0)
        return -1;
    lamport_time++;
    if (msg->s_header.s_local_time >= lamport_time)
        lamport_time = msg->s_header.s_local_time + 1;
    return 0;
}

int receive_any(void * self, Message * msg) {
    ContextPtr ctx = (ContextPtr) self;
    while (1) {
        for (int id = 0; id < ctx->host_cnt; ++id) {
            if (ctx->id == id)
                continue;
            if (receive(self, id, msg) == 0) {
                lamport_time++;
                if (msg->s_header.s_local_time >= lamport_time)
                    lamport_time = msg->s_header.s_local_time + 1;
                return 0;
            }
        }
    }
}

int receive_all(void* self, local_id min_src, MessageType status) {
    ContextPtr ctx = (ContextPtr) self;
    Message msg;
    while (min_src < ctx->host_cnt) {
        if (min_src == ctx->id) {
            min_src++;
        } else {
            int ret = ipcContextReceive(ctx->ipc, min_src, ctx->id, &msg);
            if (ret > 0 && msg.s_header.s_type == (int16_t) status) {
                lamport_time++;
                if (msg.s_header.s_local_time >= lamport_time)
                    lamport_time = msg.s_header.s_local_time + 1;
                min_src++;
            } else if (ret == -1)
                return -1;
        }
    }
    return 0;
}
