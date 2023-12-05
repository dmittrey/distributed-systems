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
            printf("In process %d\n", ret);
            Message msg;

            ContextPtr ctx = contextCreate(id, ipcCtx);
            ipcContextPrepare(ipcCtx, id);

            // Multicast Started
            startedMessage(&msg, id, getpid(), getppid());
            if (send_multicast(ctx, &msg) == 0)
                loggerProcessStarted(eventsLogger, id, getpid(), getppid());

            // Wait Started from all
            int recv_started = 0;
            // Without me
            while (recv_started != proc_cnt - 1) {
                if (receive_any(ctx, &msg) == 0 && msg.s_header.s_type == STARTED)
                    recv_started++;
            }
            loggerProcessReceivedAllStarted(eventsLogger, id);


            // Multicast Done
            doneMessage(&msg, id);
            if (send_multicast(ctx, &msg) == 0)
                loggerProcessDone(eventsLogger, id);

            // Wait Done from all
            int recv_done = 0;
            // Without me
            while (recv_done != proc_cnt - 1) {
                if (receive_any(ctx, &msg) == 0 && msg.s_header.s_type == DONE)
                    recv_done++;
            }
            loggerProcessReceivedAllDone(eventsLogger, id);

            exit(1);
        }
    }

    printf("In process!\n");

    Message msg;

    ContextPtr ctx = contextCreate(0, ipcCtx);
    ipcContextPrepare(ipcCtx, 0);

    // Wait Started from all
    loggerProcessStarted(eventsLogger, ctx->id, getpid(), getppid());
    int recv_started = 0;
    while (recv_started != proc_cnt) {
        if (receive_any(ctx, &msg) == 0 && msg.s_header.s_type == STARTED)
            recv_started++;
    }
    loggerProcessReceivedAllStarted(eventsLogger, ctx->id);

    // Wait Done from all
    loggerProcessDone(eventsLogger, ctx->id);
    int recv_done = 0;
    while (recv_done != proc_cnt) {
        if (receive_any(ctx, &msg) == 0 && msg.s_header.s_type == DONE)
            recv_done++;
    }
    loggerProcessReceivedAllDone(eventsLogger, ctx->id);

    for (int i = 0; i < proc_cnt; i++)
        wait(NULL);
    loggerDestroy(eventsLogger);
    return 0;
}
