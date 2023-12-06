#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#include "pa1.h"
#include "logger_event.h"

void loggerProcessStarted(LoggerPtr instance, local_id id, pid_t pid, pid_t ppid) {
    dprintf(instance->fd, log_started_fmt, id, pid, ppid);
    dprintf(STDOUT_FILENO, log_started_fmt, id, pid, ppid);
}
void loggerProcessReceivedAllStarted(LoggerPtr instance, local_id id) {
    dprintf(instance->fd, log_received_all_started_fmt, id);
    dprintf(STDOUT_FILENO, log_received_all_started_fmt, id);
}
void loggerProcessDone(LoggerPtr instance, local_id id) {
    dprintf(instance->fd, log_done_fmt, id);
    dprintf(STDOUT_FILENO, log_done_fmt, id);
}
void loggerProcessReceivedAllDone(LoggerPtr instance, local_id id) {
    dprintf(instance->fd, log_received_all_done_fmt, id);
    dprintf(STDOUT_FILENO, log_received_all_done_fmt, id);
}
