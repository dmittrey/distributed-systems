#pragma once

#include "banking.h"
#include "ipc_context.h"
#include "logger.h"
#include "process_state.h"

typedef uint8_t bool_t;

#define FALSE 0
#define TRUE 1

typedef struct ClientContext* ClientContextPtr;
typedef struct ServerContext* ServerContextPtr;

typedef enum ContextType {
    CLIENT = 0,
    SERVER,
} ContextType;

struct Context {
    local_id id;
    int host_cnt;
    IpcContextPtr ipc;
    ContextType type;
    ProcessStatePtr state;
    LoggerPtr events_logger;
    bool_t is_stopped;
};

struct ClientContext {
    struct Context ctx;
    AllHistory history;
};

struct ServerContext {
    struct Context ctx;
    balance_t balance;
    BalanceHistory b_history;
};

void contextDestroy(ContextPtr instance);
ProcessStateType contextStateType(ContextPtr instance);

ClientContextPtr clientContext(ContextPtr instance);
ServerContextPtr serverContext(ContextPtr instance);

int receiveAll(ContextPtr instance, local_id min_src, MessageType status);

/* ClientContextPtr */

ClientContextPtr clientContextCreate(local_id id, int host_cnt, IpcContextPtr ipc, LoggerPtr events_logger);

int multicastStopMsg(ClientContextPtr instance);

int sendTransfer(ClientContextPtr instance, local_id src, local_id dst, balance_t amount);

void appendBalanceHistory(ClientContextPtr instance, BalanceHistory *balance_history);

/* ServerContextPtr */

ServerContextPtr serverContextCreate(local_id id, int host_cnt, IpcContextPtr ipc, balance_t balance, LoggerPtr events_logger);

int multicastStartedMsg(ServerContextPtr instance);
int multicastDoneMsg(ServerContextPtr instance);

int sendAckMsg(ServerContextPtr instance, local_id dst);
int sendBalanceHistory(ServerContextPtr instance, local_id dst);

void decreaseBalance(ServerContextPtr instance, balance_t amount);
void increaseBalance(ServerContextPtr instance, balance_t amount);
void prepareBalance(ServerContextPtr instance);
