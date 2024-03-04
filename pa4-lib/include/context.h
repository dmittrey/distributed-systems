#pragma once

#include "banking.h"
#include "ipc_ctx.h"
#include "logger.h"
#include "state.h"
#include "queue.h"

typedef enum ContextType {
    ROOT = 0,
    CHILD,
} ContextType;

struct Context {
    local_id id;
    int host_cnt;
    IpcContextPtr ipc;
    ContextType type;
    StatePtr state;
    QueuePtr queue;
    LoggerPtr events_logger;
    bool_t is_mutexl;
};

ContextPtr contextCreate(local_id id, int host_cnt, IpcContextPtr ipc, LoggerPtr events_logger, ContextType type, bool_t is_mutexl);
void contextDestroy(ContextPtr instance);

StateType contextStateType(ContextPtr instance);

int receive_any_with_sender(void* instance, local_id *sender, Message * msg);
int receive_all(void* instance, local_id min_src, MessageType status);

int multicastStartedMsg(ContextPtr instance);
int multicastDoneMsg(ContextPtr instance);

int sendCSReply(ContextPtr instance, local_id dst);
