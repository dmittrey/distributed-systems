#define _GNU_SOURCE 
#include <stdio.h>

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "logger.h"
#include "pa2345.h"

#include "banking.h"

static const char * const log_pipe_closed_i_fmt =
    "Process %1d CLOSED_I pipe (%d)\n";

static const char * const log_pipe_closed_o_fmt =
    "Process %1d CLOSED_O pipe (%d)\n";

static const char * const log_pipe_closed_io_fmt =
    "Process %1d CLOSED_IO pipe (%d, %d)\n";

LoggerPtr loggerCreate(const char *const file_path) {
    LoggerPtr instance = (LoggerPtr) malloc(sizeof(struct Logger));
    instance->fd = open(file_path, O_CREAT | O_APPEND | O_WRONLY , 0644);
    return instance;
}
void loggerDestroy(LoggerPtr instance) {
    close(instance->fd);
    free(instance);
}

void loggerChannelIClosed(LoggerPtr instance, local_id id, ChannelPtr channel) {
    dprintf(instance->fd, log_pipe_closed_i_fmt, id, channel->fd[0]);
}
void loggerChannelOClosed(LoggerPtr instance, local_id id, ChannelPtr channel) {
    dprintf(instance->fd, log_pipe_closed_o_fmt, id, channel->fd[1]);
}
void loggerChannelIOClosed(LoggerPtr instance, local_id id, ChannelPtr channel) {
    dprintf(instance->fd, log_pipe_closed_io_fmt, id, channel->fd[0], channel->fd[1]);
}

void loggerProcessStarted(LoggerPtr instance, local_id id, pid_t pid, pid_t ppid) {
    dprintf(instance->fd, log_started_fmt, get_lamport_time(), id, pid, ppid, 0);
    dprintf(STDOUT_FILENO, log_started_fmt, get_lamport_time(), id, pid, ppid, 0);
}
void loggerProcessReceivedAllStarted(LoggerPtr instance, local_id id) {
    dprintf(instance->fd, log_received_all_started_fmt, get_lamport_time(), id);
    dprintf(STDOUT_FILENO, log_received_all_started_fmt, get_lamport_time(), id);
}
void loggerProcessDone(LoggerPtr instance, local_id id) {
    dprintf(instance->fd, log_done_fmt, get_lamport_time(), id, 0);
    dprintf(STDOUT_FILENO, log_done_fmt, get_lamport_time(), id, 0);
}
void loggerProcessReceivedAllDone(LoggerPtr instance, local_id id) {
    dprintf(instance->fd, log_received_all_done_fmt, get_lamport_time(), id);
    dprintf(STDOUT_FILENO, log_received_all_done_fmt, get_lamport_time(), id);
}
