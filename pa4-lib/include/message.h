#pragma once

#include <unistd.h>
#include <sys/types.h>

#include "ipc.h"
#include "banking.h"

void startedMessage(Message *msg, local_id id, pid_t pid, pid_t ppid, balance_t balance);
void doneMessage(Message *msg, local_id id, balance_t balance);
void ackMessage(Message *msg);
void stopMessage(Message *msg);
void transferMessage(Message *msg, local_id src, local_id dst, balance_t amount);
void balanceHistoryMessage(Message *msg, BalanceHistory *balance_history);
