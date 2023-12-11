#include <assert.h>
#include <stdio.h>

#include "banking.h"
#include "message.h"
#include "context.h"

void transfer(void * parent_data, local_id src, local_id dst,
              balance_t amount)
{
    Message msg;
    transferMessage(&msg, src, dst, amount);

    assert(send(parent_data, src, &msg) == 0);
    
    while (1) {
        if (receive(parent_data, dst, &msg) == 0) {
            if (msg.s_header.s_type == ACK) {
                break;
            }
        }
    }
}
