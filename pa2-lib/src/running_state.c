#include <assert.h>
#include <stdio.h>

#include "process_state.h"
#include "reporting_state.h"

#include "context.h"
#include "banking.h"
#include "message.h"
#include "logger_event.h"

static void recvTransfer(ContextPtr instance, Message* msg);
static void recvStop(ContextPtr instance);
static void recvDone(ContextPtr instance);

void transitionToRunningState(ContextPtr instance) {
    processStateDefaultImpl(instance);
    instance->state->recv_transfer = recvTransfer;
    instance->state->recv_stop = recvStop;
    instance->state->recv_done = recvDone;
    instance->state->type = STATE_RUNNING;

    if (instance->type == CLIENT) {
        bank_robbery(instance, instance->host_cnt - 1);
        if (multicastStopMsg((ClientContextPtr)instance) == 0)
            instance->is_stopped = TRUE;
    }
}

static void recvTransfer(ContextPtr instance, Message* msg) {
    TransferOrder* transfer = (TransferOrder*)msg->s_payload;
    if (instance->id == transfer->s_src) {
        decreaseBalance((ServerContextPtr)instance, transfer->s_amount);
        if (send(instance, transfer->s_dst, msg) == 0)
            loggerProcessTransferOut(instance->events_logger, transfer->s_src, transfer->s_dst, transfer->s_amount);
    }
    if (instance->id == transfer->s_dst) {
        increaseBalance((ServerContextPtr)instance, transfer->s_amount);
        if (sendAckMsg((ServerContextPtr)instance, 0) == 0)
            loggerProcessTransferIn(instance->events_logger, transfer->s_src, transfer->s_dst, transfer->s_amount);
    }
}
static void recvStop(ContextPtr instance) {
    if (instance->type == SERVER) {
        instance->is_stopped = TRUE;
        if (multicastDoneMsg((ServerContextPtr)instance) == 0)
            loggerProcessDone(instance->events_logger, instance->id, serverContext(instance)->balance);
    }
}
static void recvDone(ContextPtr instance) {
    instance->state->done_cnt++;
    if (instance->is_stopped && instance->state->done_cnt == instance->host_cnt - (instance->type == CLIENT ? 1 : 2)) {
        loggerProcessReceivedAllDone(instance->events_logger, instance->id);
        transitionToReportingState(instance);
    }
}
