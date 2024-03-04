#include <stdio.h>

#include "state_started.h"
#include "state_request_cs.h"
#include "state_half_done.h"

#include "pa2345.h"

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
        if (instance->is_mutexl == FALSE) {
            for (int i = 1; i < instance->id * 5 + 1; i++) {
                char buf[100];
                loopMessage(buf, instance->id, i, instance->id * 5);
                print(buf);
            }
            transitionToHalfDoneState(instance);
            return;
        }
        
        transitionToRequestCSState(instance);
    }
}
