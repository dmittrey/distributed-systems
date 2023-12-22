#include <stdio.h>

#include "context.h"

void transitionToDoneState(ContextPtr instance) {
    processStateDefaultImpl(instance);
    instance->state->type = STATE_DONE;

    if (instance->type == CLIENT) {
        print_history(&clientContext(instance)->history);
    }
}
