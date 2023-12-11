#include <stdio.h>
#include <stdlib.h>

#include "process/process_state.h"

static void defaultRecvStarted(ProcessStatePtr instance);
static void defaultRecvTransfer(ProcessStatePtr instance, Message *msg);
static void defaultRecvStop(ProcessStatePtr instance);
static void defaultRecvDone(ProcessStatePtr instance);
static void defaultRecvBalanceHistory(ProcessStatePtr instance, Message *msg);

void processStateDefaultImpl(ProcessStatePtr instance) {
    instance->recv_started = defaultRecvStarted;
    instance->recv_transfer = defaultRecvTransfer;
    instance->recv_stop = defaultRecvStop;
    instance->recv_done = defaultRecvDone;
    instance->recv_balance_hist = defaultRecvBalanceHistory;
}

ProcessStatePtr processStateCreate(local_id id, IpcContextPtr ipc, LoggerPtr events_logger, balance_t balance) {
    ProcessStatePtr ptr = malloc(sizeof(struct ProcessState));
    ptr->state = STATE_NONE;
    ptr->proc_ctx = contextCreate(id, ipc);
    ptr->events_logger = events_logger;
    ptr->started_cnt = 0;
    ptr->done_cnt = 0;
    ptr->balance_hist_cnt = 0;
    ptr->is_stopped = FALSE;
    if (id == 0) {
        ptr->all_history = malloc(sizeof(AllHistory));
        ptr->all_history->s_history_len = 0;
    } else {
        ptr->all_history = NULL;
    }
    ptr->b_history = malloc(sizeof(BalanceHistory));
    ptr->b_history->s_id = id;
    ptr->b_history->s_history_len = 1;
    ptr->b_history->s_history[0] = (BalanceState){.s_balance = balance,
                                                  .s_balance_pending_in = 0,
                                                  .s_time = get_physical_time()};
    return ptr;
}
void processStateDestroy(ProcessStatePtr instance) {
    contextDestroy(instance->proc_ctx);
    if (instance->proc_ctx->id == 0)
        free(instance->all_history);
    else
        free(instance->b_history);
    free(instance);
}

balance_t processStateGetBalance(ProcessStatePtr instance) {
    return instance->b_history->s_history[instance->b_history->s_history_len - 1].s_balance;
}

static void defaultRecvStarted(ProcessStatePtr instance) {
    printf("%d: %d: Stub for defaultRecvStarted", instance->proc_ctx->id, instance->state);
}

static void defaultRecvTransfer(ProcessStatePtr instance, Message* msg) {
    (void)msg;
    printf("%d: %d: Stub for defaultRecvTransfer", instance->proc_ctx->id, instance->state);
}

static void defaultRecvStop(ProcessStatePtr instance) {
    printf("%d: %d: Stub for defaultRecvStop", instance->proc_ctx->id, instance->state);
}

static void defaultRecvDone(ProcessStatePtr instance) {
    printf("%d: %d: Stub for defaultRecvDone", instance->proc_ctx->id, instance->state);
}

static void defaultRecvBalanceHistory(ProcessStatePtr instance, Message* msg) {
    (void)msg;
    printf("%d: %d: Stub for defaultRecvBalanceHistory", instance->proc_ctx->id, instance->state);
}