#pragma once

#include <sys/types.h>

#include "banking.h"
#include "logger.h"

void loggerProcessStarted(LoggerPtr instance, local_id id, pid_t pid, pid_t ppid, balance_t balance);
void loggerProcessReceivedAllStarted(LoggerPtr instance, local_id id);
void loggerProcessDone(LoggerPtr instance, local_id id, balance_t balance);
void loggerProcessTransferOut(LoggerPtr instance, local_id src, local_id dst, balance_t amount);
void loggerProcessTransferIn(LoggerPtr instance, local_id src, local_id dst, balance_t amount);
void loggerProcessReceivedAllDone(LoggerPtr instance, local_id id);
