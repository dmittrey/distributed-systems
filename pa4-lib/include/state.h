#pragma once

#include "ipc.h"

typedef uint8_t bool_t;

#define FALSE 0
#define TRUE 1

typedef struct Context* ContextPtr;

typedef enum StateType {
    STATE_NONE = 0,
    STATE_STARTED,
    STATE_REQUEST_CS,
    STATE_PENDING_CS,
    STATE_RUNNING_CS,
    STATE_RELEASE_CS,
    STATE_HALF_DONE,
    STATE_DONE,
} StateType;

/*
                        STATE_DONE
                            /\
                            |
    STATE_STARTED -> STATE_REQUEST_CS -> STATE_PENDING_CS 
                        /\                   |
                        |                   \/
               STATE_RELEASE_CS <- STATE_RUNNING_CS
*/
typedef struct State* StatePtr;

typedef void (*EventRecvStartedFunc)(ContextPtr);
typedef void (*EventRecvCSFunc)(ContextPtr, timestamp_t, local_id);
typedef void (*EventRecvDoneFunc)(ContextPtr);

struct State {
    StateType type;
    EventRecvStartedFunc recv_started;
    EventRecvCSFunc recv_cs_request;
    EventRecvCSFunc recv_cs_reply;
    EventRecvCSFunc recv_cs_release;
    EventRecvDoneFunc recv_done;
    /**/
    int req_num;
    /**/
    int done_cnt;
};

void stateDefaultImpl(ContextPtr);

StatePtr stateCreate();
void stateDestroy(StatePtr instance);
