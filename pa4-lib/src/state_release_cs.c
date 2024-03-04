#include "state_release_cs.h"
#include "state_request_cs.h"

#include "pa2345.h"

#include "context.h"

void transitionToReleaseCSState(ContextPtr instance) {
    // printf("%1d: Entered to STATE_RELEASE_CS\n", instance->id);

    stateDefaultImpl(instance);
    instance->state->type = STATE_RELEASE_CS;

    release_cs(instance);

    transitionToRequestCSState(instance);
}
