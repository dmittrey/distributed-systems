#pragma once

#include <sys/types.h>

#include "banking.h"
#include "channel.h"

typedef struct Logger *LoggerPtr;

struct Logger {
    int fd;
};

LoggerPtr loggerCreate(const char *const file_path);
void loggerDestroy(LoggerPtr instance);

void loggerChannelIClosed(LoggerPtr instance, local_id id, ChannelPtr channel);
void loggerChannelOClosed(LoggerPtr instance, local_id id, ChannelPtr channel);
void loggerChannelIOClosed(LoggerPtr instance, local_id id, ChannelPtr channel);

void loggerProcessStarted(LoggerPtr instance, local_id id, pid_t pid, pid_t ppid, balance_t balance);
void loggerProcessReceivedAllStarted(LoggerPtr instance, local_id id);
void loggerProcessDone(LoggerPtr instance, local_id id, balance_t balance);
void loggerProcessTransferOut(LoggerPtr instance, local_id src, local_id dst, balance_t amount);
void loggerProcessTransferIn(LoggerPtr instance, local_id src, local_id dst, balance_t amount);
void loggerProcessReceivedAllDone(LoggerPtr instance, local_id id);
