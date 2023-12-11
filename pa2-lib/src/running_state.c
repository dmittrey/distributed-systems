#include <assert.h>
#include <stdio.h>

#include "process_state.h"
#include "reporting_state.h"

#include "banking.h"
#include "message.h"
#include "logger_event.h"

static void recvTransfer(ProcessStatePtr instance, Message* msg);
static void recvStop(ProcessStatePtr instance);
static void recvDone(ProcessStatePtr instance);

void transitionToRunningState(ProcessStatePtr instance) {
    processStateDefaultImpl(instance);
    instance->recv_transfer = recvTransfer;
    instance->recv_stop = recvStop;
    instance->recv_done = recvDone;
    instance->state = STATE_RUNNING;

    if (instance->proc_ctx->id == 0) {
        bank_robbery(instance->proc_ctx, instance->proc_ctx->ipc->proc_cnt - 1);

        Message msg;
        stopMessage(&msg);
        if (send_multicast(instance->proc_ctx, &msg) == 0) {
            instance->is_stopped = TRUE;
            loggerProcessDone(instance->events_logger, instance->proc_ctx->id, processStateGetBalance(instance));
        }
    }
}

static void recvTransfer(ProcessStatePtr instance, Message* msg) {
    TransferOrder* transfer = (TransferOrder*)msg->s_payload;
    BalanceState balance = instance->b_history->s_history[instance->b_history->s_history_len - 1];
    if (instance->proc_ctx->id == transfer->s_src) {
        // printf("========== Enter %d %d\n", instance->b_history->s_history_len, get_physical_time());
        BalanceState new_balance = (BalanceState){.s_balance = balance.s_balance - transfer->s_amount,
                                                  .s_balance_pending_in = 0,
                                                  .s_time = get_physical_time()};
        instance->b_history->s_history[instance->b_history->s_history_len] = new_balance;
        instance->b_history->s_history_len++;
        if (send(instance->proc_ctx, transfer->s_dst, msg) == 0)
            loggerProcessTransferOut(instance->events_logger, transfer->s_src, transfer->s_dst, transfer->s_amount);
    } 
    if (instance->proc_ctx->id == transfer->s_dst) {
        // printf("========== Outer %d\n", instance->b_history->s_history_len);
        BalanceState new_balance = (BalanceState){.s_balance = balance.s_balance + transfer->s_amount,
                                                  .s_balance_pending_in = 0,
                                                  .s_time = get_physical_time()};
        instance->b_history->s_history[instance->b_history->s_history_len] = new_balance;
        instance->b_history->s_history_len++;

        Message ack_msg;
        ackMessage(&ack_msg);
        if (send(instance->proc_ctx, 0, &ack_msg) == 0)
            loggerProcessTransferIn(instance->events_logger, transfer->s_src, transfer->s_dst, transfer->s_amount);
    }
}
static void recvStop(ProcessStatePtr instance) {
    printf("%d: Receied stop!\n", instance->proc_ctx->id);
    instance->is_stopped = TRUE;
    Message msg;
    doneMessage(&msg, instance->proc_ctx->id, processStateGetBalance(instance));
    if (send_multicast(instance->proc_ctx, &msg) == 0)
        loggerProcessDone(instance->events_logger, instance->proc_ctx->id, processStateGetBalance(instance));
}
static void recvDone(ProcessStatePtr instance) {
    instance->done_cnt++;
    if (instance->is_stopped && instance->done_cnt == instance->proc_ctx->ipc->proc_cnt - (instance->proc_ctx->id == 0 ? 1 : 2)) {
        loggerProcessReceivedAllDone(instance->events_logger, instance->proc_ctx->id);
        transitionToReportingState(instance);
    }
}