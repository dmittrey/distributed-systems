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
    increaseBalance(instance, 0, 0);
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
    Message msg;
    prepareBalance(instance);
    balanceHistoryMessage(&msg, &instance->b_history);
    return send(instance, dst, &msg);
}

void decreaseBalance(ServerContextPtr instance, balance_t amount) {
    BalanceState state = (BalanceState){.s_balance = instance->balance,
                                        .s_balance_pending_in = 0};
    while (instance->b_history.s_history_len < get_lamport_time()) {
        state.s_time = instance->b_history.s_history_len;
        instance->b_history.s_history[instance->b_history.s_history_len] = state;
        instance->b_history.s_history_len += 1;
    }
    instance->balance -= amount;
    state.s_balance = instance->balance;
    state.s_time = get_lamport_time();
    instance->b_history.s_history[instance->b_history.s_history_len] = state;
    instance->b_history.s_history_len += 1;
}
void increaseBalance(ServerContextPtr instance, timestamp_t send_time, balance_t amount) {
    printf("Increase with send_time = %d to %d, hs = %d\n", send_time, get_lamport_time(), instance->b_history.s_history_len);
    BalanceState state = (BalanceState){.s_balance = instance->balance,
                                        .s_balance_pending_in = 0};
    while (instance->b_history.s_history_len < get_lamport_time()) {
        state.s_time = instance->b_history.s_history_len;
        instance->b_history.s_history[instance->b_history.s_history_len] = state;
        instance->b_history.s_history_len += 1;
        if (state.s_time >= send_time - 1)
            instance->b_history.s_history[state.s_time].s_balance_pending_in += amount;
    }
    instance->balance += amount;
    state.s_balance = instance->balance;
    state.s_time = get_lamport_time();
    instance->b_history.s_history[instance->b_history.s_history_len] = state;
    instance->b_history.s_history_len += 1;
}
void prepareBalance(ServerContextPtr instance) {
    instance->b_history.s_history_len = get_lamport_time() + 1;
    for (int i = 0; i < instance->b_history.s_history_len; ++i) {
        if (instance->b_history.s_history[i + 1].s_time == 0) {
            instance->b_history.s_history[i + 1] = instance->b_history.s_history[i];
            instance->b_history.s_history[i + 1].s_time = i + 1;
        }
    }
}
