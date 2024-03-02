#pragma once

#include <sys/types.h>

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

void loggerProcessStarted(LoggerPtr instance, local_id id, pid_t pid, pid_t ppid);
void loggerProcessReceivedAllStarted(LoggerPtr instance, local_id id);

void loggerProcessDone(LoggerPtr instance, local_id id);
void loggerProcessReceivedAllDone(LoggerPtr instance, local_id id);
