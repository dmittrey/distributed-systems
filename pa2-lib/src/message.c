#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "pa1.h"
#include "message.h"

void startedMessage(Message *msg, local_id id, pid_t pid, pid_t ppid) {
    int size = sprintf(msg->s_payload, log_started_fmt, id, pid, ppid);
    msg->s_header.s_type = STARTED;
    msg->s_header.s_payload_len = size;
}
void doneMessage(Message *msg, local_id id) {
    int size = sprintf(msg->s_payload, log_done_fmt, id);
    msg->s_header.s_type = DONE;
    msg->s_header.s_payload_len = size;
}
