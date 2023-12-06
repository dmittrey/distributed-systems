#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "pa1.h"
#include "message.h"

void startedMessage(Message *msg, local_id id, pid_t pid, pid_t ppid) {
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    long current_time = currentTime.tv_sec * 1000 + currentTime.tv_usec / 1000;

    int size = sprintf(msg->s_payload, log_started_fmt, current_time, id, pid, ppid);
    msg->s_header.s_type = STARTED;
    msg->s_header.s_payload_len = size;
}
void doneMessage(Message *msg, local_id id) {
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    long current_time = currentTime.tv_sec * 1000 + currentTime.tv_usec / 1000;
    
    int size = sprintf(msg->s_payload, log_done_fmt, current_time, id);
    msg->s_header.s_type = DONE;
    msg->s_header.s_payload_len = size;
}
