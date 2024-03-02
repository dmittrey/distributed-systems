#pragma once

#include "ipc.h"

typedef struct IpcContext* IpcContextPtr;

IpcContextPtr ipcContextCreate(int host_cnt);
void ipcContextDestroy(IpcContextPtr instance);

void ipcContextPrepare(IpcContextPtr instance, local_id id);

/*
-1          : On error
0 or above  : On success
*/
int ipcContextSend(const IpcContextPtr instance, local_id src, local_id dst, const Message *msg);
/*
-1          : On error
0 or above  : Payload size in msg
*/
int ipcContextReceive(const IpcContextPtr instance, local_id src, local_id dst, Message *msg);
