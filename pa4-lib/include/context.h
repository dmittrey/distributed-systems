#pragma once

#include "banking.h"
#include "ipc_ctx.h"
#include "logger.h"
#include "state.h"

typedef uint8_t bool_t;

#define FALSE 0
#define TRUE 1

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
    LoggerPtr events_logger;
};

ContextPtr contextCreate(local_id id, int host_cnt, IpcContextPtr ipc, LoggerPtr events_logger, ContextType type);
void contextDestroy(ContextPtr instance);

StateType contextStateType(ContextPtr instance);

int receive_all(void* instance, local_id min_src, MessageType status);

int multicastStartedMsg(ContextPtr instance);
int multicastDoneMsg(ContextPtr instance);
