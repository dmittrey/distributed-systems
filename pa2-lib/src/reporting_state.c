#include <string.h>
#include <stdio.h>

#include "process_state.h"
#include "done_state.h"

#include "context.h"
#include "message.h"

static void recvBalanceHistory(ContextPtr instance, Message* msg);

void transitionToReportingState(ContextPtr instance) {
    processStateDefaultImpl(instance);
    instance->state->recv_balance_hist = recvBalanceHistory;
    instance->state->type = STATE_REPORTING;

    if (instance->type == SERVER && sendBalanceHistory((ServerContextPtr)instance, 0) == 0)
        transitionToDoneState(instance);
}

static void recvBalanceHistory(ContextPtr instance, Message* msg) {
    if (instance->type == CLIENT) {
        BalanceHistory *b_history = (BalanceHistory*) msg->s_payload;
        appendBalanceHistory((ClientContextPtr)instance, b_history);

        instance->state->balance_history_cnt++;
        if (instance->state->balance_history_cnt == instance->host_cnt - 1)
            transitionToDoneState(instance);
    }
}
