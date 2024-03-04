#include "state_running_cs.h"
#include "state_release_cs.h"
#include "state_request_cs.h"

#include "context.h"
#include "message.h"
#include "pa2345.h"

void transitionToRunningCSState(ContextPtr instance) {
    // printf("%1d: Entered to STATE_RUNNING_CS\n", instance->id);

    stateDefaultImpl(instance);
    instance->state->type = STATE_RUNNING_CS;

    char buf[100];
    loopMessage(buf, instance->id, instance->state->req_num, instance->id * 5);
    print(buf);

    transitionToReleaseCSState(instance);
}
