#include <stdio.h>

#include "process_state.h"
#include "running_state.h"

#include "context.h"
#include "message.h"
#include "logger_event.h"

static void recvStarted(ContextPtr instance);

void transitionToStartedState(ContextPtr instance) {
    processStateDefaultImpl(instance);
    instance->state->recv_started = recvStarted;
    instance->state->type = STATE_STARTED;

    if (instance->type == SERVER && multicastStartedMsg((ServerContextPtr)instance) == 0)
        loggerProcessStarted(instance->events_logger, instance->id, getpid(), getppid(),
                             ((ServerContextPtr)instance)->balance);
}

static void recvStarted(ContextPtr instance) {
    instance->state->started_cnt++;
    if (instance->state->started_cnt == instance->host_cnt - (instance->type == CLIENT ? 1 : 2)) {
        loggerProcessReceivedAllStarted(instance->events_logger, instance->id);
        transitionToRunningState(instance);
    }
}
