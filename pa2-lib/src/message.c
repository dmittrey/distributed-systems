#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

#include "pa2345.h"
#include "message.h"

void startedMessage(Message *msg, local_id id, pid_t pid, pid_t ppid, balance_t balance) {
    int size = sprintf(msg->s_payload, log_started_fmt, get_physical_time(), id, pid, ppid, balance);
    msg->s_header.s_type = STARTED;
    msg->s_header.s_payload_len = size;
    msg->s_header.s_local_time = get_physical_time();
}
void doneMessage(Message *msg, local_id id, balance_t balance) {
    int size = sprintf(msg->s_payload, log_done_fmt, get_physical_time(), id, balance);
    msg->s_header.s_type = DONE;
    msg->s_header.s_payload_len = size;
    msg->s_header.s_local_time = get_physical_time();
}
void ackMessage(Message *msg) {
    msg->s_header.s_type = ACK;
    msg->s_header.s_payload_len = 0;
    msg->s_header.s_local_time = get_physical_time();
}
void stopMessage(Message *msg) {
    msg->s_header.s_type = STOP;
    msg->s_header.s_payload_len = 0;
    msg->s_header.s_local_time = get_physical_time();
}
void transferMessage(Message *msg, local_id src, local_id dst, balance_t amount) {
    TransferOrder transfer = (TransferOrder) {.s_src = src, .s_dst = dst, .s_amount = amount};
    memcpy(msg->s_payload, &transfer, sizeof(TransferOrder));
    msg->s_header.s_type = TRANSFER;
    msg->s_header.s_payload_len = sizeof(TransferOrder);
    msg->s_header.s_local_time = get_physical_time();
}
void balanceHistoryMessage(Message *msg, BalanceHistory *balance_history) {
    memcpy(msg->s_payload, balance_history, sizeof(BalanceHistory));
    msg->s_header.s_type = BALANCE_HISTORY;
    msg->s_header.s_payload_len = sizeof(BalanceHistory);
    msg->s_header.s_local_time = get_physical_time();
}
