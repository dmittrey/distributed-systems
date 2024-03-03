#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h> 

#include "common.h"

#include "state.h"
#include "state_started.h"

#include "ipc_ctx.h"
#include "context.h"

int validate(int argc, char **argv, int *proc_cnt, bool_t *is_mutexl)
{
    *is_mutexl = FALSE;
    if (argc < 3)
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

    if (argc == 4 && strcmp(argv[3], "--mutexl") == 0) 
    {
        *is_mutexl = TRUE;
        printf("DEBUG: Mutex enabled!\n");
    } else if (argc == 4) {
        printf("ERROR: Wrong argument!\n");
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
    bool_t is_mutexl = FALSE;

    if ((err = validate(argc, argv, &proc_cnt, &is_mutexl)))
        return err;
    
    IpcContextPtr ipcCtx = ipcContextCreate(proc_cnt + 1);
    LoggerPtr eventsLogger = loggerCreate(events_log);
    for (int id = 1; id < proc_cnt + 1; ++id) {
        pid_t ret = fork();
        if (!ret) {
            ContextPtr server = contextCreate(id, proc_cnt + 1, ipcCtx, eventsLogger, CHILD);
            ipcContextPrepare(ipcCtx, id);
            transitionToStartedState(server);

            Message msg;
            local_id sender;
            while (contextStateType(server) != STATE_DONE) {
                if (receive_any_with_sender(server, &sender, &msg) == 0) {
                    switch (msg.s_header.s_type) {
                    case CS_REQUEST:
                        server->state->recv_cs_request(server, msg.s_header.s_local_time, sender);
                        break;
                    case CS_REPLY:
                        server->state->recv_cs_reply(server, msg.s_header.s_local_time, sender);
                        break;
                    case CS_RELEASE:
                        server->state->recv_cs_release(server, msg.s_header.s_local_time, sender);
                        break;
                    }
                }
            }

            contextDestroy((ContextPtr)server);
            exit(1);
        }
    }

    ContextPtr client = contextCreate(0, proc_cnt + 1, ipcCtx, eventsLogger, ROOT);
    ipcContextPrepare(ipcCtx, 0);
    transitionToStartedState((ContextPtr)client);

    while (contextStateType(client) != STATE_DONE);

    loggerDestroy(eventsLogger);
    ipcContextDestroy(ipcCtx);
    contextDestroy((ContextPtr)client);

    for (int i = 0; i < proc_cnt; i++)
        wait(NULL);

    return 0;
}
