#pragma once

#include "ipc_context.h"

typedef struct Context* ContextPtr;

struct Context {
    local_id id;
    IpcContextPtr ipc;
};

ContextPtr contextCreate(local_id id, IpcContextPtr ipc);
void contextDestroy(ContextPtr instance);
