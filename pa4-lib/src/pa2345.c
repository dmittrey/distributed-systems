#include <stdio.h>

#include "banking.h"
#include "message.h"
#include "context.h"

extern timestamp_t lamport_time;

timestamp_t get_lamport_time() {
    return lamport_time;
}
