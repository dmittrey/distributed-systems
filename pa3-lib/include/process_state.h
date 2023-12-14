#pragma once

#include "ipc.h"

typedef struct Context* ContextPtr;

typedef enum ProcessStateType {
    STATE_NONE = 0,
    STATE_STARTED,
    STATE_RUNNING,
    STATE_REPORTING,
    STATE_DONE,
} ProcessStateType;

/*
    started -> running -> reporting -> done
*/
typedef struct ProcessState* ProcessStatePtr;

typedef void (*EventRecvStartedFunc)(ContextPtr);
typedef void (*EventRecvTransferFunc)(ContextPtr, Message*);
typedef void (*EventRecvStopFunc)(ContextPtr);
typedef void (*EventRecvDoneFunc)(ContextPtr);
typedef void (*EventRecvBalanceHistoryFunc)(ContextPtr, Message*);

struct ProcessState {
    ProcessStateType type;
    EventRecvStartedFunc recv_started;
    EventRecvTransferFunc recv_transfer;
    EventRecvStopFunc recv_stop;
    EventRecvDoneFunc recv_done;
    EventRecvBalanceHistoryFunc recv_balance_hist;
    /**/
    int started_cnt;
    int done_cnt;
    int balance_history_cnt;
};

void processStateDefaultImpl(ContextPtr);

ProcessStatePtr processStateCreate();
void processStateDestroy(ProcessStatePtr instance);
