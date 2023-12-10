#pragma once

#include <sys/types.h>

#include "ipc.h"
#include "logger.h"

void loggerProcessStarted(LoggerPtr instance, local_id id, pid_t pid, pid_t ppid);
void loggerProcessReceivedAllStarted(LoggerPtr instance, local_id id);
void loggerProcessDone(LoggerPtr instance, local_id id);
void loggerProcessReceivedAllDone(LoggerPtr instance, local_id id);
