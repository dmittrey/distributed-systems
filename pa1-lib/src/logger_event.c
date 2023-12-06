#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#include "pa1.h"
#include "logger_event.h"

void loggerProcessStarted(LoggerPtr instance, local_id id, pid_t pid, pid_t ppid) {
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    long current_time = currentTime.tv_sec * 1000 + currentTime.tv_usec / 1000;

    dprintf(instance->fd, log_started_fmt, current_time, id, pid, ppid);
    dprintf(STDOUT_FILENO, log_started_fmt, current_time, id, pid, ppid);
}
void loggerProcessReceivedAllStarted(LoggerPtr instance, local_id id) {
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    long current_time = currentTime.tv_sec * 1000 + currentTime.tv_usec / 1000;

    dprintf(instance->fd, log_received_all_started_fmt, current_time, id);
    dprintf(STDOUT_FILENO, log_received_all_started_fmt, current_time, id);
}
void loggerProcessDone(LoggerPtr instance, local_id id) {
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    long current_time = currentTime.tv_sec * 1000 + currentTime.tv_usec / 1000;

    dprintf(instance->fd, log_done_fmt, current_time, id);
    dprintf(STDOUT_FILENO, log_done_fmt, current_time, id);
}
void loggerProcessReceivedAllDone(LoggerPtr instance, local_id id) {
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    long current_time = currentTime.tv_sec * 1000 + currentTime.tv_usec / 1000;

    dprintf(instance->fd, log_received_all_done_fmt, current_time, id);
    dprintf(STDOUT_FILENO, log_received_all_done_fmt, current_time, id);
}
