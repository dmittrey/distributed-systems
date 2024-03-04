#pragma once

#include <unistd.h>
#include <sys/types.h>

#include "ipc.h"

/* Start state msgs*/
void startedMessage(Message *msg, local_id id, pid_t pid, pid_t ppid);

/* Running state msgs*/
void CSRequestMessage(Message *msg);
void CSReplyMessage(Message *msg);
void CSReleaseMessage(Message *msg);
void loopMessage(char* buf, local_id id, int iter_num, int iter_cnt);

/* Done state msgs */
void doneMessage(Message *msg, local_id id);
