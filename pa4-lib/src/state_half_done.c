#include <stdio.h>

#include "state.h"
#include "state_half_done.h"

#include "context.h"
#include "message.h"
#include "logger.h"

void transitionToHalfDoneState(ContextPtr instance) {
    // printf("%1d: Entered to STATE_HALF_DONE\n", instance->id);
    stateDefaultImpl(instance);
    instance->state->type = STATE_HALF_DONE;

    if (instance->type == CHILD && multicastDoneMsg(instance) == 0) {
        // loggerProcessDone(instance->events_logger, instance->id);
    }

    if (instance->type == ROOT) {
        loggerProcessDone(instance->events_logger, instance->id);

        if (receive_all(instance, 1, DONE) == 0) {
            loggerProcessReceivedAllDone(instance->events_logger, instance->id);
        }
    }

    if (instance->state->done_cnt == instance->host_cnt - 2) {
        loggerProcessReceivedAllDone(instance->events_logger, instance->id);
        instance->state->type = STATE_DONE;
    }
}
