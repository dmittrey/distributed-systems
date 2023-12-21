#include <assert.h>
#include <stdio.h>

#include "process_state.h"
#include "reporting_state.h"

#include "context.h"
#include "banking.h"
#include "message.h"
#include "logger.h"

static void recvTransfer(ContextPtr instance, Message* msg);
static void recvStop(ContextPtr instance);

void transitionToRunningState(ContextPtr instance) {
    printf("%d Transition to running with: %d\n", instance->id, get_lamport_time());
    processStateDefaultImpl(instance);
    instance->state->recv_transfer = recvTransfer;
    instance->state->recv_stop = recvStop;
    instance->state->type = STATE_RUNNING;

    if (instance->type == CLIENT) {
        bank_robbery(instance, instance->host_cnt - 1);
        assert(multicastStopMsg((ClientContextPtr)instance) == 0);
        if (receive_all(instance, 1, DONE) == 0) {
            loggerProcessReceivedAllDone(instance->events_logger, instance->id);
            transitionToReportingState(instance);
        }
    }
}

static void recvTransfer(ContextPtr instance, Message* msg) {
    printf("%d Receive transfer with: %d\n", instance->id, get_lamport_time());
    TransferOrder* transfer = (TransferOrder*)msg->s_payload;
    if (instance->id == transfer->s_src && instance->is_stopped == FALSE) {
        msg->s_header.s_local_time = get_lamport_time();
        decreaseBalance((ServerContextPtr)instance, transfer->s_amount);
        if (send(instance, transfer->s_dst, msg) == 0)
            loggerProcessTransferOut(instance->events_logger, transfer->s_src, transfer->s_dst, transfer->s_amount);
    }
    if (instance->id == transfer->s_dst) {
        increaseBalance((ServerContextPtr)instance, msg->s_header.s_local_time, transfer->s_amount);
        if (sendAckMsg((ServerContextPtr)instance, 0) == 0)
            loggerProcessTransferIn(instance->events_logger, transfer->s_src, transfer->s_dst, transfer->s_amount);
    }
}
static void recvStop(ContextPtr instance) {
    if (instance->type == SERVER) {
        instance->is_stopped = TRUE;
        if (multicastDoneMsg((ServerContextPtr)instance) == 0)
            loggerProcessDone(instance->events_logger, instance->id, serverContext(instance)->balance);

        if (receive_all(instance, 1, DONE) == 0) {
            loggerProcessReceivedAllDone(instance->events_logger, instance->id);
            transitionToReportingState(instance);
        }
    }
}
