#include <string.h>
#include <stdio.h>

#include "process/process_state.h"
#include "process/done_state.h"

#include "message.h"

static void recvBalanceHistory(ProcessStatePtr instance, Message* msg);

void transitionToReportingState(ProcessStatePtr instance) {
    processStateDefaultImpl(instance);
    instance->recv_balance_hist = recvBalanceHistory;
    instance->state = STATE_REPORTING;

    if (instance->proc_ctx->id != 0) {
        Message msg;
        balanceHistoryMessage(&msg, instance->b_history);
        if (send(instance->proc_ctx, 0, &msg) == 0) {
            transitionToDoneState(instance);
        }
    }
}

static void recvBalanceHistory(ProcessStatePtr instance, Message* msg) {
    if (instance->proc_ctx->id == 0) {
        instance->all_history->s_history[instance->all_history->s_history_len] = *(BalanceHistory*)msg->s_payload;
        // printf("Redawdawdawdawdawdawdawd: %d", instance->all_history->s_history[instance->all_history->s_history_len].s_history_len);
        instance->all_history->s_history_len++;
        if (instance->all_history->s_history_len == instance->proc_ctx->ipc->proc_cnt - 1)
            transitionToDoneState(instance);
    }
}