#include <stdio.h>

#include "process_state.h"
#include "running_state.h"

#include "context.h"
#include "message.h"
#include "logger.h"

void transitionToStartedState(ContextPtr instance) {
    processStateDefaultImpl(instance);
    instance->state->type = STATE_STARTED;

    if (instance->type == SERVER && multicastStartedMsg((ServerContextPtr)instance) == 0) {
        loggerProcessStarted(instance->events_logger, instance->id, getpid(), getppid(),
                             ((ServerContextPtr)instance)->balance);
    }

    if (receive_all(instance, 1, STARTED) == 0) {
        loggerProcessReceivedAllStarted(instance->events_logger, instance->id);
        transitionToRunningState(instance);
    }
}
