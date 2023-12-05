#pragma once

#include "ipc.h"
#include <unistd.h>
#include <sys/types.h>

void startedMessage(Message *msg, local_id id, pid_t pid, pid_t ppid);
void doneMessage(Message *msg, local_id id);
