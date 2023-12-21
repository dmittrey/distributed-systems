#define _GNU_SOURCE 
#include <stdio.h>

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "logger.h"
#include "pa2345.h"
#include "pipe.h"

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

void loggerProcessStarted(LoggerPtr instance, local_id id, pid_t pid, pid_t ppid, balance_t balance) {
    dprintf(instance->fd, log_started_fmt, get_lamport_time(), id, pid, ppid, balance);
    dprintf(STDOUT_FILENO, log_started_fmt, get_lamport_time(), id, pid, ppid, balance);
}
void loggerProcessReceivedAllStarted(LoggerPtr instance, local_id id) {
    dprintf(instance->fd, log_received_all_started_fmt, get_lamport_time(), id);
    dprintf(STDOUT_FILENO, log_received_all_started_fmt, get_lamport_time(), id);
}
void loggerProcessDone(LoggerPtr instance, local_id id, balance_t balance) {
    dprintf(instance->fd, log_done_fmt, get_lamport_time(), id, balance);
    dprintf(STDOUT_FILENO, log_done_fmt, get_lamport_time(), id, balance);
}
void loggerProcessTransferOut(LoggerPtr instance, local_id src, local_id dst, balance_t amount) {
    dprintf(instance->fd, log_transfer_out_fmt, get_lamport_time(), src, amount, dst);
    dprintf(STDOUT_FILENO, log_transfer_out_fmt, get_lamport_time(), src, amount, dst);
}
void loggerProcessTransferIn(LoggerPtr instance, local_id src, local_id dst, balance_t amount) {
    dprintf(instance->fd, log_transfer_in_fmt, get_lamport_time(), dst, amount, src);
    dprintf(STDOUT_FILENO, log_transfer_in_fmt, get_lamport_time(), dst, amount, src);
}
void loggerProcessReceivedAllDone(LoggerPtr instance, local_id id) {
    dprintf(instance->fd, log_received_all_done_fmt, get_lamport_time(), id);
    dprintf(STDOUT_FILENO, log_received_all_done_fmt, get_lamport_time(), id);
}
