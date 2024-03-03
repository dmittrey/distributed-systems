#include <stdio.h>
#include <stdlib.h>

#include "context.h"
#include "state.h"

static void defaultRecvStarted(ContextPtr instance);
static void defaultRecvCSRequest(ContextPtr instance, timestamp_t l_time, local_id sender_id);
static void defaultRecvCSReply(ContextPtr instance, timestamp_t l_time, local_id sender_id);
static void defaultRecvCSRelease(ContextPtr instance, timestamp_t l_time, local_id sender_id);
static void defaultRecvDone(ContextPtr instance);

void stateDefaultImpl(ContextPtr instance) {
    instance->state->recv_started = defaultRecvStarted;
    instance->state->recv_cs_request = defaultRecvCSRequest;
    instance->state->recv_cs_reply = defaultRecvCSReply;
    instance->state->recv_cs_release = defaultRecvCSRelease;
    instance->state->recv_done = defaultRecvDone;
}

StatePtr stateCreate() {
    StatePtr ptr = malloc(sizeof(struct State));
    ptr->is_req = FALSE;
    ptr->is_aprvd = FALSE;
    ptr->req_num = 1;
    ptr->reply_cnt = 0;
    return ptr;
}
void stateDestroy(StatePtr instance) {
    free(instance);
}

static void defaultRecvStarted(ContextPtr instance) {
    printf("%1d: %d: Stub for defaultRecvStarted\n", instance->id, instance->state->type);
}

static void defaultRecvCSRequest(ContextPtr instance, timestamp_t l_time, local_id sender_id) {
    printf("%1d: %d: Stub for defaultRecvCSRequest (%d, %1d)\n", instance->id, instance->state->type, l_time, sender_id);
}

static void defaultRecvCSReply(ContextPtr instance, timestamp_t l_time, local_id sender_id) {
    printf("%1d: %d: Stub for defaultRecvCSReply (%d, %1d)\n", instance->id, instance->state->type, l_time, sender_id);
}

static void defaultRecvCSRelease(ContextPtr instance, timestamp_t l_time, local_id sender_id) {
    printf("%1d: %d: Stub for defaultRecvCSRelease (%d, %1d)\n", instance->id, instance->state->type, l_time, sender_id);
}

static void defaultRecvDone(ContextPtr instance) {
    printf("%1d: %d: Stub for defaultRecvDone\n", instance->id, instance->state->type);
}
