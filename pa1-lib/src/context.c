#include <stdlib.h>

#include "context.h"

ContextPtr contextCreate(local_id id, IpcContextPtr ipc) {
    ContextPtr instance = (ContextPtr) malloc(sizeof(struct Context));
    instance->id = id;
    instance->ipc = ipc;
    return instance;
}
void contextDestroy(ContextPtr instance) {
    free(instance);
}
