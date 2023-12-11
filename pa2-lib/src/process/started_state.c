#include <stdio.h>

#include "process/process_state.h"
#include "process/running_state.h"

#include "message.h"
#include "logger_event.h"

static void recvStarted(ProcessStatePtr instance);

void transitionToStartedState(ProcessStatePtr instance) {
    processStateDefaultImpl(instance);
    instance->recv_started = recvStarted;
    instance->state = STATE_STARTED;

    if (instance->proc_ctx->id == 0) {
        loggerProcessStarted(instance->events_logger, instance->proc_ctx->id, getpid(), getppid(), processStateGetBalance(instance));
    } else {
        Message msg;
        startedMessage(&msg, instance->proc_ctx->id, getpid(), getppid(), processStateGetBalance(instance));
        if (send_multicast(instance->proc_ctx, &msg) == 0)
            loggerProcessStarted(instance->events_logger, instance->proc_ctx->id, getpid(), getppid(), processStateGetBalance(instance));
    }
}

static void recvStarted(ProcessStatePtr instance) {
    instance->started_cnt++;
    if (instance->started_cnt == instance->proc_ctx->ipc->proc_cnt - (instance->proc_ctx->id == 0 ? 1 : 2)) {
        loggerProcessReceivedAllStarted(instance->events_logger, instance->proc_ctx->id);
        transitionToRunningState(instance);
    }
}