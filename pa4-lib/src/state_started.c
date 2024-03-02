#include <stdio.h>

#include "state.h"
#include "state_running.h"

#include "context.h"
#include "message.h"
#include "logger.h"

void transitionToStartedState(ContextPtr instance) {
    stateDefaultImpl(instance);
    instance->state->type = STATE_STARTED;

    if (instance->type == CHILD && multicastStartedMsg(instance) == 0) {
        loggerProcessStarted(instance->events_logger, instance->id, getpid(), getppid());
    }

    if (instance->type == ROOT) {
        loggerProcessStarted(instance->events_logger, instance->id, getpid(), getppid());
    }

    if (receive_all(instance, 1, STARTED) == 0) {
        loggerProcessReceivedAllStarted(instance->events_logger, instance->id);
        transitionToRunningState(instance);
    }
}
