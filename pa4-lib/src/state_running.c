#include <assert.h>
#include <stdio.h>

#include "state.h"
#include "state_done.h"

#include "context.h"

void transitionToRunningState(ContextPtr instance) {
    stateDefaultImpl(instance);
    instance->state->type = STATE_RUNNING;

    transitionToDoneState(instance);
}
