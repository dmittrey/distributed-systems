#pragma once

#include "ipc.h"

typedef uint8_t bool_t;

#define FALSE 0
#define TRUE 1

typedef struct Context* ContextPtr;

typedef enum StateType {
    STATE_NONE = 0,
    STATE_STARTED,
    STATE_RUNNING,
    STATE_DONE,
} StateType;

/*
    started -> running -> done
*/
typedef struct State* StatePtr;

typedef void (*EventRecvStartedFunc)(ContextPtr);
typedef void (*EventRecvCSRequestFunc)(ContextPtr, timestamp_t, local_id);
typedef void (*EventRecvCSReplyFunc)(ContextPtr, timestamp_t, local_id);
typedef void (*EventRecvCSReleaseFunc)(ContextPtr, timestamp_t, local_id);
typedef void (*EventRecvDoneFunc)(ContextPtr);

struct State {
    StateType type;
    EventRecvStartedFunc recv_started;
    EventRecvCSRequestFunc recv_cs_request;
    EventRecvCSReplyFunc recv_cs_reply;
    EventRecvCSReleaseFunc recv_cs_release;
    EventRecvDoneFunc recv_done;
    /**/
    bool_t is_req;
    bool_t is_aprvd;
    int req_num;
    int reply_cnt;
};

void stateDefaultImpl(ContextPtr);

StatePtr stateCreate();
void stateDestroy(StatePtr instance);
