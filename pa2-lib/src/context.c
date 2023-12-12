#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "context.h"
#include "message.h"

void contextDestroy(ContextPtr instance) {
    processStateDestroy(instance->state);
    free(instance);
}
ProcessStateType contextStateType(ContextPtr instance) {
    return instance->state->type;
}

ClientContextPtr clientContext(ContextPtr instance) {
    return (ClientContextPtr) instance;
}

ServerContextPtr serverContext(ContextPtr instance) {
    return (ServerContextPtr) instance;
}

ClientContextPtr clientContextCreate(local_id id, int host_cnt, IpcContextPtr ipc, LoggerPtr events_logger) {
    ClientContextPtr instance = (ClientContextPtr) malloc(sizeof(struct ClientContext));
    instance->ctx.id = id;
    instance->ctx.host_cnt = host_cnt;
    instance->ctx.ipc = ipc;
    instance->ctx.type = CLIENT;
    instance->ctx.state = processStateCreate();
    instance->ctx.events_logger = events_logger;
    instance->ctx.is_stopped = FALSE;
    instance->history.s_history_len = 0;
    return instance;
}

int multicastStopMsg(ClientContextPtr instance) {
    Message msg;
    stopMessage(&msg);
    return send_multicast(instance, &msg);
}

int sendTransfer(ClientContextPtr instance, local_id src, local_id dst, balance_t amount) {
    Message msg;
    transferMessage(&msg, src, dst, amount);
    return send(instance, src, &msg) == 0;
}

void appendBalanceHistory(ClientContextPtr instance, BalanceHistory *balance_history) {
    instance->history.s_history[instance->history.s_history_len] = *balance_history;
    instance->history.s_history_len++;
}

ServerContextPtr serverContextCreate(local_id id, int host_cnt, IpcContextPtr ipc, balance_t balance, LoggerPtr events_logger) {
    ServerContextPtr instance = (ServerContextPtr) malloc(sizeof(struct ServerContext));
    instance->ctx.id = id;
    instance->ctx.host_cnt = host_cnt;
    instance->ctx.ipc = ipc;
    instance->ctx.type = SERVER;
    instance->ctx.state = processStateCreate();
    instance->ctx.events_logger = events_logger;
    instance->ctx.is_stopped = FALSE;
    instance->balance = balance;
    instance->b_history.s_history_len = 0;
    instance->b_history.s_id = id;
    increaseBalance(instance, 0);
    return instance;
}

int multicastStartedMsg(ServerContextPtr instance) {
    Message msg;
    startedMessage(&msg, instance->ctx.id, getpid(), getppid(), instance->balance);
    return send_multicast(instance, &msg);
}
int multicastDoneMsg(ServerContextPtr instance) {
    Message msg;
    doneMessage(&msg, instance->ctx.id, instance->balance);
    return send_multicast(instance, &msg);
}

int sendAckMsg(ServerContextPtr instance, local_id dst) {
    Message msg;
    ackMessage(&msg);
    return send(instance, dst, &msg);
}
int sendBalanceHistory(ServerContextPtr instance, local_id dst) {
    printf("Sending balance history from %d to %d\n", instance->ctx.id, dst);
    Message msg;
    balanceHistoryMessage(&msg, &instance->b_history);
    return send(instance, dst, &msg);
}

void decreaseBalance(ServerContextPtr instance, balance_t amount) {
    instance->balance -= amount;
    instance->b_history.s_history[instance->b_history.s_history_len] = (BalanceState){.s_balance = instance->balance,
                                                                                      .s_balance_pending_in = 0,
                                                                                      .s_time = get_physical_time()};
    instance->b_history.s_history_len += 1;
    printf("Add balance state (amount = %d, time = %d)\n", instance->balance, get_physical_time());
}
void increaseBalance(ServerContextPtr instance, balance_t amount) {
    instance->balance += amount;
    instance->b_history.s_history[instance->b_history.s_history_len] = (BalanceState){.s_balance = instance->balance,
                                                                                      .s_balance_pending_in = 0,
                                                                                      .s_time = get_physical_time()};
    instance->b_history.s_history_len += 1;
    printf("Add balance state (amount = %d, time = %d)\n", instance->balance, get_physical_time());
}
