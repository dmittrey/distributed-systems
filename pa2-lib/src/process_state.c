#include <stdio.h>
#include <stdlib.h>

#include "context.h"
#include "process_state.h"

static void defaultRecvStarted(ContextPtr instance);
static void defaultRecvTransfer(ContextPtr instance, Message *msg);
static void defaultRecvStop(ContextPtr instance);
static void defaultRecvDone(ContextPtr instance);
static void defaultRecvBalanceHistory(ContextPtr instance, Message *msg);

void processStateDefaultImpl(ContextPtr instance) {
    instance->state->recv_started = defaultRecvStarted;
    instance->state->recv_transfer = defaultRecvTransfer;
    instance->state->recv_stop = defaultRecvStop;
    instance->state->recv_done = defaultRecvDone;
    instance->state->recv_balance_hist = defaultRecvBalanceHistory;
}

ProcessStatePtr processStateCreate() {
    ProcessStatePtr ptr = malloc(sizeof(struct ProcessState));
    return ptr;
}
void processStateDestroy(ProcessStatePtr instance) {
    free(instance);
}

static void defaultRecvStarted(ContextPtr instance) {
    printf("%d: %d: Stub for defaultRecvStarted\n", instance->id, instance->state->type);
}

static void defaultRecvTransfer(ContextPtr instance, Message* msg) {
    (void)msg;
    printf("%d: %d: Stub for defaultRecvTransfer\n", instance->id, instance->state->type);
}

static void defaultRecvStop(ContextPtr instance) {
    printf("%d: %d: Stub for defaultRecvStop\n", instance->id, instance->state->type);
}

static void defaultRecvDone(ContextPtr instance) {
    printf("%d: %d: Stub for defaultRecvDone\n", instance->id, instance->state->type);
}

static void defaultRecvBalanceHistory(ContextPtr instance, Message* msg) {
    (void)msg;
    printf("%d: %d: Stub for defaultRecvBalanceHistory\n", instance->id, instance->state->type);
}
