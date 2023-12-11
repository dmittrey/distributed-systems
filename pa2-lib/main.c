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
            ProcessStatePtr state = processStateCreate(id, ipcCtx, eventsLogger, atoi(argv[id + 2]));
            ipcContextPrepare(ipcCtx, id);
            transitionToStartedState(state);

            Message msg;
            local_id cur = 1;
            while (state->state != STATE_DONE) {
                // sleep(1);
                printf("%d: cur = %d\n", state->proc_ctx->id, cur);
                int ret = -1;
                if (state->state == STATE_RUNNING) {
                    ret = receive_any(state->proc_ctx, &msg);
                } else {
                    if (cur == id)
                        ++cur;
                    else if (cur > proc_cnt)
                        cur = 1;
                    else 
                        if ((ret = receive(state->proc_ctx, cur, &msg)))
                            ++cur;
                }

                if (ret == 0) {
                    switch (msg.s_header.s_type)
                    {
                    case STARTED:
                        state->recv_started(state);
                        break;
                    case DONE: 
                        state->recv_done(state);
                        break;
                    case STOP:     
                        state->recv_stop(state);
                        break;       
                    case TRANSFER:     
                        state->recv_transfer(state, &msg);
                        break;   
                    case BALANCE_HISTORY:
                        state->recv_balance_hist(state, &msg);
                        break;
                    default:
                        break;
                    }
                }
            }

            // printf("%d: EXITED\n", state->proc_ctx->id);

            processStateDestroy(state);
            exit(1);
        }
    }

    ProcessStatePtr state = processStateCreate(0, ipcCtx, eventsLogger, 0);
    ipcContextPrepare(ipcCtx, 0);
    transitionToStartedState(state);

    Message msg;
    local_id cur = 1;
    while (state->state != STATE_DONE)
    {
        if (cur > proc_cnt)
            cur = 1;
        else if (receive(state->proc_ctx, cur, &msg) == 0) {
            switch (msg.s_header.s_type) {
            case STARTED:
                state->recv_started(state);
                break;
            case DONE:
                state->recv_done(state);
                break;
            case STOP:
                state->recv_stop(state);
                break;
            case TRANSFER:
                state->recv_transfer(state, &msg);
                break;
            case BALANCE_HISTORY:
                state->recv_balance_hist(state, &msg);
                break;
            default:
                break;
            }
            ++cur;
        }
    }

    loggerDestroy(eventsLogger);
    ipcContextDestroy(ipcCtx);
    processStateDestroy(state);

    for (int i = 0; i < proc_cnt; i++)
        wait(NULL);
    return 0;
}
