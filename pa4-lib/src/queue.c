#include "queue.h"

struct Queue {
    QueueItemPtr front;
};

static struct QueueItem* createQueueItem(timestamp_t l_time, local_id id) {
    struct QueueItem* newItem = (struct QueueItem*)malloc(sizeof(struct QueueItem));
    newItem->ltime = l_time;
    newItem->id = id;
    newItem->next = NULL;
    return newItem;
}

QueuePtr queueCreate() {
    QueuePtr q = (QueuePtr)malloc(sizeof(struct Queue));
    q->front = NULL;
    return q;
}
void queueDestroy(QueuePtr q) {
    while (q->front != NULL) {
        QueueItemPtr temp = q->front;
        q->front = q->front->next;
        free(temp);
    }
    free(q);
}

void queuePush(QueuePtr q, timestamp_t ltime, local_id id) {
    QueueItemPtr newEntry = createQueueItem(ltime, id);
    
    if (q->front == NULL) {
        q->front = newEntry;
    } else {
        QueueItemPtr cur = q->front;
        QueueItemPtr prev = NULL;

        while (cur != NULL && 
               (cur->ltime < ltime || (cur->ltime == ltime && cur->id < id))) {
            prev = cur;
            cur = cur->next;
        }

        if (prev == NULL) {
            newEntry->next = q->front;
            q->front = newEntry;
        } else {
            prev->next = newEntry;
            newEntry->next = cur;
        }
    }
}
QueueItemPtr queueGet(QueuePtr q) {
    return q->front;
}
QueueItemPtr queuePop(QueuePtr q) {
    if (q->front == NULL) {
        return NULL;
    }
    
    QueueItemPtr temp = q->front;
    q->front = q->front->next;
    free(temp);
    
    return temp;
}

void queuePrint(QueuePtr q, local_id id) {
    printf("Queue print for %1d: \n", id);
    if (q->front == NULL) {
        printf("Queue is empty\n");
        return;
    }
    
    QueueItemPtr current = q->front;
    int i = 1;
    while (current != NULL) {
        printf("%d: Timestamp: %d, ID: %d\n", i++, current->ltime, current->id);
        current = current->next;
    }
}