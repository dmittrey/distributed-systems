#pragma once

#include "channel.h"
#include "logger.h"

typedef struct IpcContext* IpcContextPtr;

struct IpcContext {
    int proc_cnt;
    struct Channel **channels;
    LoggerPtr channelLogger;
};

IpcContextPtr ipcContextCreate(int proc_cnt);
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
/*
-1          : On error
0           : On success
*/
int ipcContextReceiveAll(const IpcContextPtr instance, local_id dst, local_id min_src, MessageType status);
