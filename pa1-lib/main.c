#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
            int started = 0;
            int done = 0;

            ContextPtr ctx = contextCreate(id, ipcCtx);
            ipcContextPrepare(ipcCtx, id);

            // Multicast Started
            startedMessage(&msg, id, getpid(), getppid());
            if (send_multicast(ctx, &msg) == 0)
                loggerProcessStarted(eventsLogger, id, getpid(), getppid());

            // Wait Started from all children
            while (started != proc_cnt - 1) {
                if (receive_any(ctx, &msg) == 0) {
                    if (msg.s_header.s_type == STARTED)
                        started++;
                    if (msg.s_header.s_type == DONE)
                        done++;
                }
            }
            loggerProcessReceivedAllStarted(eventsLogger, id);

            // Multicast Done
            doneMessage(&msg, id);
            if (send_multicast(ctx, &msg) == 0)
                loggerProcessDone(eventsLogger, id);

            // Wait Done from all children
            while (done < proc_cnt - 1) {
                if (receive_any(ctx, &msg) == 0) {
                    if (msg.s_header.s_type == DONE)
                        done++;
                }
            }
            loggerProcessReceivedAllDone(eventsLogger, id);

            contextDestroy(ctx);
            exit(1);
        }
    }

    Message msg;
    int started = 0;
    int done = 0;

    ContextPtr ctx = contextCreate(0, ipcCtx);
    // ipcContextPrepare(ipcCtx, 0);

    // Wait Started from all
    loggerProcessStarted(eventsLogger, ctx->id, getpid(), getppid());
    while (started != proc_cnt) {
        if (receive_any(ctx, &msg) == 0) {
            if (msg.s_header.s_type == STARTED)
                    started++;
            if (msg.s_header.s_type == DONE)
                    done++;
        }
    }
    loggerProcessReceivedAllStarted(eventsLogger, ctx->id);

    // Wait Done from all
    loggerProcessDone(eventsLogger, ctx->id);
    while (done != proc_cnt) {
        if (receive_any(ctx, &msg) == 0) {
            if (msg.s_header.s_type == DONE)
                done++;
        }
    }
    loggerProcessReceivedAllDone(eventsLogger, ctx->id);

    for (int i = 0; i < proc_cnt; i++)
        wait(NULL);
    loggerDestroy(eventsLogger);
    contextDestroy(ctx);
    ipcContextDestroy(ipcCtx);
    return 0;
}
