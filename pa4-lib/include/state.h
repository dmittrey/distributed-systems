#pragma once

#include "ipc.h"

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
typedef void (*EventRecvDoneFunc)(ContextPtr);

struct State {
    StateType type;
    EventRecvStartedFunc recv_started;
    EventRecvDoneFunc recv_done;
    /**/
    int started_cnt;
    int done_cnt;
};

void stateDefaultImpl(ContextPtr);

StatePtr stateCreate();
void stateDestroy(StatePtr instance);
