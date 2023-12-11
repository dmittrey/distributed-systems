#pragma once

#include "context.h"
#include "banking.h"

typedef uint8_t bool_t;

#define FALSE 0
#define TRUE 1

typedef enum ProcessStateEnum {
    STATE_NONE = 0,
    STATE_STARTED,
    STATE_RUNNING,
    STATE_REPORTING,
    STATE_DONE,
} ProcessStateEnum;

/*
    started -> running -> reporting -> done
*/
typedef struct ProcessState* ProcessStatePtr;

typedef void (*EventRecvStartedFunc)(ProcessStatePtr);
typedef void (*EventRecvTransferFunc)(ProcessStatePtr, Message*);
typedef void (*EventRecvStopFunc)(ProcessStatePtr);
typedef void (*EventRecvDoneFunc)(ProcessStatePtr);
typedef void (*EventRecvBalanceHistoryFunc)(ProcessStatePtr, Message*);

struct ProcessState {
    ProcessStateEnum state;
    ContextPtr proc_ctx;
    LoggerPtr events_logger;
    int started_cnt;
    int done_cnt;
    int balance_hist_cnt;
    /**/
    bool_t is_stopped;
    BalanceHistory *b_history;
    AllHistory *all_history;
    /**/
    EventRecvStartedFunc recv_started;
    EventRecvTransferFunc recv_transfer;
    EventRecvStopFunc recv_stop;
    EventRecvDoneFunc recv_done;
    EventRecvBalanceHistoryFunc recv_balance_hist;
};

void processStateDefaultImpl(ProcessStatePtr);

ProcessStatePtr processStateCreate(local_id id, IpcContextPtr ipc, LoggerPtr events_logger, balance_t balance);
void processStateDestroy(ProcessStatePtr instance);

balance_t processStateGetBalance(ProcessStatePtr);