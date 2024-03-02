#include <stdio.h>

#include "state.h"
#include "state_done.h"

#include "context.h"
#include "message.h"
#include "logger.h"

void transitionToDoneState(ContextPtr instance) {
    stateDefaultImpl(instance);
    instance->state->type = STATE_DONE;

    if (instance->type == CHILD && multicastDoneMsg(instance) == 0) {
        loggerProcessDone(instance->events_logger, instance->id);
    }

    if (instance->type == ROOT) {
        loggerProcessStarted(instance->events_logger, instance->id, getpid(), getppid());
    }

    if (receive_all(instance, 1, DONE) == 0) {
        loggerProcessReceivedAllDone(instance->events_logger, instance->id);
    }
}
