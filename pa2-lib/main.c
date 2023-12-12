#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "common.h"
#include "process_state.h"
#include "started_state.h"
#include "ipc_context.h"
#include "context.h"

int validate(int argc, char **argv, int *proc_cnt)
{
    if (argc < 4)
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
    if (argc != 3 + *proc_cnt)
    {
        printf("ERROR: Specify balance for every proc!\n");
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
            ServerContextPtr server = serverContextCreate(id, proc_cnt + 1, ipcCtx, atoi(argv[id + 2]), eventsLogger);
            ipcContextPrepare(ipcCtx, id);
            transitionToStartedState((ContextPtr)server);

            Message msg;
            local_id cur = 1;
            while (contextStateType((ContextPtr)server) != STATE_DONE) {
                int ret = -1;
                if (contextStateType((ContextPtr)server) == STATE_RUNNING) {
                    ret = receive_any(server, &msg);
                } else {
                    if (cur == id)
                        ++cur;
                    else if (cur > proc_cnt)
                        cur = 1;
                    else 
                        if ((ret = receive(server, cur, &msg)))
                            ++cur;
                }

                if (ret == 0) {
                    switch (msg.s_header.s_type)
                    {
                    case STARTED:
                        server->ctx.state->recv_started((ContextPtr)server);
                        break;
                    case DONE: 
                        server->ctx.state->recv_done((ContextPtr)server);
                        break;
                    case STOP:     
                        server->ctx.state->recv_stop((ContextPtr)server);
                        break;       
                    case TRANSFER:     
                        server->ctx.state->recv_transfer((ContextPtr)server, &msg);
                        break;   
                    case BALANCE_HISTORY:
                        server->ctx.state->recv_balance_hist((ContextPtr)server, &msg);
                        break;
                    default:
                        break;
                    }
                }
            }

            contextDestroy((ContextPtr)server);
            exit(1);
        }
    }

    ClientContextPtr client = clientContextCreate(0, proc_cnt + 1, ipcCtx, eventsLogger);
    ipcContextPrepare(ipcCtx, 0);
    transitionToStartedState((ContextPtr)client);

    Message msg;
    local_id cur = 1;
    while (contextStateType((ContextPtr)client) != STATE_DONE)
    {
        if (cur > proc_cnt)
            cur = 1;
        else if (receive(client, cur, &msg) == 0) {
            switch (msg.s_header.s_type) {
            case STARTED:
                client->ctx.state->recv_started((ContextPtr)client);
                break;
            case DONE:
                client->ctx.state->recv_done((ContextPtr)client);
                break;
            case STOP:
                client->ctx.state->recv_stop((ContextPtr)client);
                break;
            case TRANSFER:
                client->ctx.state->recv_transfer((ContextPtr)client, &msg);
                break;
            case BALANCE_HISTORY:
                client->ctx.state->recv_balance_hist((ContextPtr)client, &msg);
                break;
            default:
                break;
            }
            ++cur;
        }
    }

    loggerDestroy(eventsLogger);
    ipcContextDestroy(ipcCtx);
    contextDestroy((ContextPtr)client);

    for (int i = 0; i < proc_cnt; i++)
        wait(NULL);
    return 0;
}
