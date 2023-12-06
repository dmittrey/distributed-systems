#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "common.h"
#include "context.h"
#include "logger_event.h"
#include "message.h"

int validate(int argc, char **argv, int *proc_cnt)
{
    if (argc != 3)
    {
        printf("USAGE: ./prog -p X\n");
        return 1;
    }
    if (strcmp(argv[1], "-p"))
    {
        printf("ERROR: -p not specified!\n");
        return 1;
    }
    if ((*proc_cnt = atoi(argv[2])) == 0)
    {
        printf("ERROR: Number of proc not specified!\n");
        return 1;
    }
    if (*proc_cnt < 1 || *proc_cnt > 10)
    {
        printf("ERROR: Specify proc count in range [1:10]!\n");
        return 1;
    }

    return 0;
}

int main(int argc, char **argv) {
    int proc_cnt = 0;
    int err = 0;

    if ((err = validate(argc, argv, &proc_cnt)))
        return err;
    
    IpcContextPtr ipcCtx = ipcContextCreate(proc_cnt + 1);
    LoggerPtr eventsLogger = loggerCreate(events_log);
    for (int id = 1; id < proc_cnt + 1; ++id) {
        pid_t ret = fork();
        if (!ret) {
            Message msg;

            ContextPtr ctx = contextCreate(id, ipcCtx);
            ipcContextPrepare(ipcCtx, id);

            // Multicast Started
            startedMessage(&msg, id, getpid(), getppid());
            if (send_multicast(ctx, &msg) == 0)
                loggerProcessStarted(eventsLogger, id, getpid(), getppid());

            // Wait Started from all children
            if (ipcContextReceiveAll(ipcCtx, id, 1, STARTED) == 0)
                loggerProcessReceivedAllStarted(eventsLogger, id);

            // Multicast Done
            doneMessage(&msg, id);
            if (send_multicast(ctx, &msg) == 0)
                loggerProcessDone(eventsLogger, id);

            // Wait Done from all children
            if (ipcContextReceiveAll(ipcCtx, id, 1, DONE) == 0)
                loggerProcessReceivedAllDone(eventsLogger, id);

            loggerDestroy(eventsLogger);
            contextDestroy(ctx);
            ipcContextDestroy(ipcCtx);
            exit(0);
        }
    }

    ContextPtr ctx = contextCreate(0, ipcCtx);
    ipcContextPrepare(ipcCtx, 0);

    // Wait Started from all
    loggerProcessStarted(eventsLogger, ctx->id, getpid(), getppid());
    if (ipcContextReceiveAll(ipcCtx, 0, 1, STARTED) == 0)
        loggerProcessReceivedAllStarted(eventsLogger, 0);

    // Wait Done from all
    loggerProcessDone(eventsLogger, ctx->id);
    if (ipcContextReceiveAll(ipcCtx, 0, 1, DONE) == 0)
        loggerProcessReceivedAllDone(eventsLogger, 0);

    for (int i = 0; i < proc_cnt; i++)
        wait(NULL);
    loggerDestroy(eventsLogger);
    contextDestroy(ctx);
    ipcContextDestroy(ipcCtx);
    return 0;
}
