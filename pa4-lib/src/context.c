#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "context.h"
#include "message.h"

ContextPtr contextCreate(local_id id, int host_cnt, IpcContextPtr ipc, LoggerPtr events_logger, ContextType type, bool_t is_mutexl) {
    ContextPtr instance = (ContextPtr) malloc(sizeof(struct Context));
    instance->id = id;
    instance->host_cnt = host_cnt;
    instance->ipc = ipc;
    instance->type = type;
    instance->state = stateCreate();
    instance->queue = queueCreate();
    instance->events_logger = events_logger;
    instance->is_mutexl = is_mutexl;
    return instance;
}
void contextDestroy(ContextPtr instance) {
    stateDestroy(instance->state);
    queueDestroy(instance->queue);
    free(instance);
}

StateType contextStateType(ContextPtr instance) {
    return instance->state->type;
}

int multicastStartedMsg(ContextPtr instance) {
    Message msg;
    startedMessage(&msg, instance->id, getpid(), getppid());
    return send_multicast(instance, &msg);
}
int multicastDoneMsg(ContextPtr instance) {
    Message msg;
    doneMessage(&msg, instance->id);
    return send_multicast(instance, &msg);
}

int sendCSReply(ContextPtr instance, local_id dst) {
    Message msg;
    CSReplyMessage(&msg);
    return send(instance, dst, &msg);
}
