#include <stdio.h>
#include <stdlib.h>

#include "context.h"
#include "state.h"

static void defaultRecvStarted(ContextPtr instance);
static void defaultRecvDone(ContextPtr instance);

void stateDefaultImpl(ContextPtr instance) {
    instance->state->recv_started = defaultRecvStarted;
    instance->state->recv_done = defaultRecvDone;
}

StatePtr stateCreate() {
    StatePtr ptr = malloc(sizeof(struct State));
    ptr->started_cnt = 0;
    ptr->done_cnt = 0;
    return ptr;
}
void stateDestroy(StatePtr instance) {
    free(instance);
}

static void defaultRecvStarted(ContextPtr instance) {
    printf("%d: %d: Stub for defaultRecvStarted\n", instance->id, instance->state->type);
}

static void defaultRecvDone(ContextPtr instance) {
    printf("%d: %d: Stub for defaultRecvDone\n", instance->id, instance->state->type);
}
