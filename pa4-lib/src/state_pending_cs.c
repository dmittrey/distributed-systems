#include "state_pending_cs.h"
#include "state_running_cs.h"

#include "pa2345.h"

#include "context.h"

static void recvCSRequest(ContextPtr instance, timestamp_t l_time, local_id sender_id);
static void recvCSReply(ContextPtr instance, timestamp_t l_time, local_id sender_id);
static void recvCSRelease(ContextPtr instance, timestamp_t l_time, local_id sender_id);

void transitionToPendingCSState(ContextPtr instance) {
    // printf("%1d: Entered to STATE_PENDING_CS\n", instance->id);

    stateDefaultImpl(instance);
    instance->state->type = STATE_PENDING_CS;
    instance->state->recv_cs_request = recvCSRequest;
    instance->state->recv_cs_reply = recvCSReply;
    instance->state->recv_cs_release = recvCSRelease;
}

static void recvCSRequest(ContextPtr instance, timestamp_t l_time, local_id sender_id) {
    queuePush(instance->queue, l_time, sender_id);

    sendCSReply(instance, sender_id);
}
static void recvCSReply(ContextPtr instance, timestamp_t l_time, local_id sender_id) {
    // printf("%1d: Received REPLY from %1d with %d\n", instance->id, sender_id, l_time);
    queueRefresh(instance->queue, sender_id, l_time);

    // queuePrint(instance->queue, instance->id);

    QueueItemPtr front = queueGet(instance->queue);
    if (front->id == instance->id && front->apprvd_replies >= instance->host_cnt - 2) {
        transitionToRunningCSState(instance);
    }
}
static void recvCSRelease(ContextPtr instance, timestamp_t l_time, local_id sender_id) {
    // printf("%1d: Received RELEASE from %1d\n", instance->id, sender_id);
    queuePop(instance->queue);
    queueRefresh(instance->queue, sender_id, l_time);

    // queuePrint(instance->queue, instance->id);

    QueueItemPtr front = queueGet(instance->queue);
    if (front->id == instance->id && front->apprvd_replies >= instance->host_cnt - 2) {
        transitionToRunningCSState(instance);
    }
}
