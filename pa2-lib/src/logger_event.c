#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>

#include "pa2345.h"
#include "logger_event.h"

void loggerProcessStarted(LoggerPtr instance, local_id id, pid_t pid, pid_t ppid, balance_t balance) {
    dprintf(instance->fd, log_started_fmt, get_physical_time(), id, pid, ppid, balance);
    dprintf(STDOUT_FILENO, log_started_fmt, get_physical_time(), id, pid, ppid, balance);
}
void loggerProcessReceivedAllStarted(LoggerPtr instance, local_id id) {
    dprintf(instance->fd, log_received_all_started_fmt, get_physical_time(), id);
    dprintf(STDOUT_FILENO, log_received_all_started_fmt, get_physical_time(), id);
}
void loggerProcessDone(LoggerPtr instance, local_id id, balance_t balance) {
    dprintf(instance->fd, log_done_fmt, get_physical_time(), id, balance);
    dprintf(STDOUT_FILENO, log_done_fmt, get_physical_time(), id, balance);
}
void loggerProcessTransferOut(LoggerPtr instance, local_id src, local_id dst, balance_t amount) {
    dprintf(instance->fd, log_transfer_out_fmt, get_physical_time(), src, amount, dst);
    dprintf(STDOUT_FILENO, log_transfer_out_fmt, get_physical_time(), src, amount, dst);
}
void loggerProcessTransferIn(LoggerPtr instance, local_id src, local_id dst, balance_t amount) {
    dprintf(instance->fd, log_transfer_in_fmt, get_physical_time(), dst, amount, src);
    dprintf(STDOUT_FILENO, log_transfer_in_fmt, get_physical_time(), dst, amount, src);
}
void loggerProcessReceivedAllDone(LoggerPtr instance, local_id id) {
    dprintf(instance->fd, log_received_all_done_fmt, get_physical_time(), id);
    dprintf(STDOUT_FILENO, log_received_all_done_fmt, get_physical_time(), id);
}
