#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "banking.h"
#include "ipc.h"

typedef struct QueueItem *QueueItemPtr;

typedef struct Queue *QueuePtr;

struct QueueItem {
    timestamp_t ltime;
    local_id id;
    int apprvd_replies;
    QueueItemPtr next;
};

QueuePtr queueCreate();
void queueDestroy(QueuePtr q);

void queuePush(QueuePtr q, timestamp_t ltime, local_id id);
QueueItemPtr queuePop(QueuePtr q);

QueueItemPtr queueGet(QueuePtr q);
QueueItemPtr queueRefresh(QueuePtr q, local_id id, timestamp_t ltime);

void queuePrint(QueuePtr q, local_id id);
