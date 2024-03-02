#include <stdlib.h>
#include <stdio.h>

#include "common.h"
#include "ipc_ctx.h"
#include "logger.h"

struct IpcContext {
    int host_cnt;
    struct Channel **channels;
    LoggerPtr logger;
};

IpcContextPtr ipcContextCreate(int host_cnt) {
    IpcContextPtr context = malloc(sizeof(struct IpcContext));
    context->host_cnt = host_cnt;
    context->logger = loggerCreate(pipes_log);
    context->channels = malloc(host_cnt * sizeof(ChannelPtr));
    for (int i = 0; i < host_cnt; ++i) {
        context->channels[i] = malloc(host_cnt * sizeof(struct Channel));
        for (int j = 0; j < host_cnt; ++j) {
            if (i != j)
                channelCreate(&context->channels[i][j]);
        }
    }
    return context;
}
void ipcContextDestroy(IpcContextPtr instance) {
    loggerDestroy(instance->logger);
    for (int i = 0; i < instance->host_cnt; ++i) {
        for (int j = 0; j < instance->host_cnt; ++j) {
            if (i != j)
                channelDestroy(&instance->channels[i][j]);
        }
        free(instance->channels[i]);
    }
    free(instance->channels);
    free(instance);
}

void ipcContextPrepare(IpcContextPtr instance, local_id id) {
    for (int i = 0; i < instance->host_cnt; ++i) {
        for (int j = 0; j < instance->host_cnt; ++j) {
            if (i == j) {
                continue;
            }
            else if (id == i) {
                channelCloseI(&instance->channels[i][j]);
                loggerChannelIClosed(instance->logger, id, &instance->channels[i][j]);
            }
            else if (id == j) {
                channelCloseO(&instance->channels[i][j]);
                loggerChannelOClosed(instance->logger, id, &instance->channels[i][j]);
            }
            else {
                channelCloseIO(&instance->channels[i][j]);
                loggerChannelIOClosed(instance->logger, id, &instance->channels[i][j]);
            }
        }
    }
}

int ipcContextSend(const IpcContextPtr instance, local_id src, local_id dst, const Message *msg) {
    if (src >= instance->host_cnt || src < 0) {
        printf("ERROR: SRC out of bound in ipcContextSend! (%d)", src);
        return -1;
    }
    if (dst >= instance->host_cnt || dst < 0) {
        printf("ERROR: DST out of bound in ipcContextSend! (%d)", dst);
        return -1;
    }
    return channelWrite(&instance->channels[src][dst], msg);
}
int ipcContextReceive(const IpcContextPtr instance, local_id src, local_id dst, Message *msg) {
    if (src >= instance->host_cnt || src < 0) {
        printf("ERROR: SRC out of bound in ipcContextReceive! (%d)", src);
        return -1;
    }
    if (dst >= instance->host_cnt || dst < 0) {
        printf("ERROR: DST out of bound in ipcContextReceive! (%d)", dst);
        return -1;
    }
    return channelRead(&instance->channels[src][dst], msg);
}
