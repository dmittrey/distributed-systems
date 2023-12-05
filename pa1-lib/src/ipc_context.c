#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "common.h"
#include "ipc_context.h"
#include "logger_channel.h"

IpcContextPtr ipcContextCreate(int proc_cnt) {
    IpcContextPtr context = malloc(sizeof(struct IpcContext));
    context->proc_cnt = proc_cnt;
    context->channelLogger = loggerCreate(pipes_log);
    context->channels = malloc(proc_cnt * sizeof(ChannelPtr));
    for (int i = 0; i < proc_cnt; ++i) {
        context->channels[i] = malloc(proc_cnt * sizeof(struct Channel));
        for (int j = 0; j < proc_cnt; ++j) {
            if (i != j)
                channelCreate(&context->channels[i][j]);
        }
    }
    return context;
}
void ipcContextDestroy(IpcContextPtr instance) {
    loggerDestroy(instance->channelLogger);
    for (int i = 0; i < instance->proc_cnt; ++i) {
        for (int j = 0; j < instance->proc_cnt; ++j) {
            if (i != j)
                channelDestroy(&instance->channels[i][j]);
        }
        free(instance->channels[i]);
    }
    free(instance->channels);
    free(instance);
}

void ipcContextPrepare(IpcContextPtr instance, local_id id) {
    for (int i = 0; i < instance->proc_cnt; ++i) {
        for (int j = 0; j < instance->proc_cnt; ++j) {
            if (i == j) {
                continue;
            }
            else if (id == i) {
                channelCloseI(&instance->channels[i][j]);
                loggerChannelIClosed(instance->channelLogger, id, &instance->channels[i][j]);
            }
            else if (id == j) {
                channelCloseO(&instance->channels[i][j]);
                loggerChannelOClosed(instance->channelLogger, id, &instance->channels[i][j]);
            }
            else {
                channelCloseIO(&instance->channels[i][j]);
                loggerChannelIOClosed(instance->channelLogger, id, &instance->channels[i][j]);
            }
        }
    }
}

int ipcContextSend(const IpcContextPtr instance, local_id src, local_id dst, const Message *msg) {
    assert(src >= 0 && src < instance->proc_cnt);
    assert(dst >= 0 && src < instance->proc_cnt);
    return channelWrite(&instance->channels[src][dst], msg);
}
int ipcContextReceive(const IpcContextPtr instance, local_id src, local_id dst, Message *msg) {
    return channelRead(&instance->channels[src][dst], msg);
}
