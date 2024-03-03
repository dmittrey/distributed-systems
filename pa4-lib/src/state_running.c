#include <assert.h>
#include <stdio.h>

#include "pa2345.h"
#include "message.h"

#include "state.h"
#include "state_done.h"

#include "context.h"

static void recvCSRequest(ContextPtr instance, timestamp_t l_time, local_id sender_id);
static void recvCSReply(ContextPtr instance, timestamp_t l_time, local_id sender_id);
static void recvCSRelease(ContextPtr instance, timestamp_t l_time, local_id sender_id);

void transitionToRunningState(ContextPtr instance) {
    stateDefaultImpl(instance);
    instance->state->type = STATE_RUNNING;
    instance->state->recv_cs_request = recvCSRequest;
    instance->state->recv_cs_reply = recvCSReply;
    instance->state->recv_cs_release = recvCSRelease;

    instance->state->is_req = FALSE;
    instance->state->is_aprvd = FALSE;
    instance->state->reply_cnt = 0;

    if (instance->type == CHILD && instance->state->is_req == FALSE && instance->state->req_num < instance->id * 5 + 1) {
        if (request_cs(instance) == 0)
            instance->state->is_req = TRUE;
    }
}

static void recvCSRequest(ContextPtr instance, timestamp_t l_time, local_id sender_id) {
    queuePush(instance->queue, l_time, sender_id);

    Message msg;
    CSReplyMessage(&msg);
    send(instance, sender_id, &msg);
}

static void recvCSReply(ContextPtr instance, timestamp_t l_time, local_id sender_id) {
    printf("%1d: Received CS Reply from %1d\n", instance->id, sender_id);
    //-2
    instance->state->reply_cnt++;

    QueueItemPtr front = queueGet(instance->queue);
    if (front->id == instance->id) {
        if (front->ltime < l_time) {
            instance->state->is_aprvd = TRUE;
        }
    }

    if (instance->state->reply_cnt == instance->host_cnt - 2) {
        // printf("%1d: Status: %d\n", instance->id, instance->state->is_aprvd);
        // queuePrint(instance->queue, instance->id);

        if (instance->state->is_req && instance->state->is_aprvd) {
            // printf("%1d: In CS!\n", instance->id);
            char buf[100];
            loopMessage(buf, instance->id, instance->state->req_num++, instance->id * 5);
            print(buf);
            release_cs(instance);
            queuePrint(instance->queue, instance->id);
            transitionToRunningState(instance);
        }
    }
}

static void recvCSRelease(ContextPtr instance, timestamp_t l_time, local_id sender_id) {
    printf("%1d: Received CS Release from %1d\n", instance->id, sender_id);
    queuePop(instance->queue);
    queuePrint(instance->queue, instance->id);
}
