#include <assert.h>
#include <stdio.h>

#include "pa2345.h"
#include "message.h"

#include "state.h"
#include "state_done.h"

#include "context.h"

void transitionToRunningState(ContextPtr instance) {
    stateDefaultImpl(instance);
    instance->state->type = STATE_RUNNING;

    if (instance->type == CHILD) {
        char buf[100];
        for (int i = 1; i < instance->id * 5 + 1; i++) {
            loopMessage(buf, instance->id, i, instance->id * 5);
            print(buf);
        }
    }

    transitionToDoneState(instance);
}
