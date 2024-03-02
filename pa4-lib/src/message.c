#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

#include "pa2345.h"
#include "message.h"

void startedMessage(Message *msg, local_id id, pid_t pid, pid_t ppid) {
    int size = sprintf(msg->s_payload, log_started_fmt, get_lamport_time(), id, pid, ppid, 0);
    msg->s_header.s_magic = MESSAGE_MAGIC;
    msg->s_header.s_type = STARTED;
    msg->s_header.s_payload_len = size;
    msg->s_header.s_local_time = get_lamport_time();
}
void doneMessage(Message *msg, local_id id) {
    int size = sprintf(msg->s_payload, log_done_fmt, get_lamport_time(), id, 0);
    msg->s_header.s_magic = MESSAGE_MAGIC;
    msg->s_header.s_type = DONE;
    msg->s_header.s_payload_len = size;
    msg->s_header.s_local_time = get_lamport_time();
}
