#pragma once

#include <unistd.h>
#include <sys/types.h>

#include "ipc.h"
#include "banking.h"

/* Start state msgs*/
void startedMessage(Message *msg, local_id id, pid_t pid, pid_t ppid);

/* Running state msgs*/
void stopMessage(Message *msg);

/* Done state msgs */
void doneMessage(Message *msg, local_id id);