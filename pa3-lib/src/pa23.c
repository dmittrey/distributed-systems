#include <stdio.h>

#include "banking.h"
#include "message.h"
#include "context.h"

extern timestamp_t lamport_time;

void transfer(void * parent_data, local_id src, local_id dst,
              balance_t amount)
{
    lamport_time++;
    sendTransfer(parent_data, src, dst, amount);
    Message msg;
    while (1) {
        if (receive(parent_data, dst, &msg) == 0) {
            lamport_time++;
            if (msg.s_header.s_type == ACK) {
                break;
            }
        }
    }
}

timestamp_t get_lamport_time() {
    return lamport_time;
}
