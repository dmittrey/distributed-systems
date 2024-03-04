#include "state_request_cs.h"
#include "state_pending_cs.h"
#include "state_half_done.h"
#include "state_running_cs.h"

#include "pa2345.h"

#include "context.h"

void transitionToRequestCSState(ContextPtr instance) {
    // printf("%1d: Entered to STATE_REQUEST_CS\n", instance->id);

    stateDefaultImpl(instance);
    instance->state->type = STATE_REQUEST_CS;

    if (instance->type == CHILD && instance->state->req_num++ < instance->id * 5) {
        if (request_cs(instance) == 0)
            transitionToPendingCSState(instance);
    } else {
        transitionToHalfDoneState(instance);
    }
}
