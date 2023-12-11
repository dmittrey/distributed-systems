#include <stdio.h>

#include "process/process_state.h"

void transitionToDoneState(ProcessStatePtr instance) {
    processStateDefaultImpl(instance);

    if (instance->proc_ctx->id == 0) {
        print_history(instance->all_history);
    }

    instance->state = STATE_DONE;
}