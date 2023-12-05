#include <assert.h>

#include "common.h"
#include "ipc.h"
#include "ipc_context.h"

void IpcContextTest_WriteFromFirstToSecond_SucessfulReadFromSecond() {
    LoggerPtr logger = loggerCreate(pipes_log);
    IpcContextPtr context = ipcContextCreate(2);

    Message msg = (Message) {.s_header.s_magic = 555, .s_header.s_payload_len = 55};
    int ret = ipcContextSend(context, 0, 1, &msg);
    assert(ret != -1);
    Message recv_msg;
    ret = ipcContextReceive(context, 0, 1, &recv_msg);
    assert(ret != -1);
    assert(recv_msg.s_header.s_magic == 555);
    assert(recv_msg.s_header.s_payload_len == 55);


    loggerDestroy(logger);
    ipcContextDestroy(context);
}

void IpcContextTest_WriteFromFirstToThird_SucessfulReadFromThird() {
    IpcContextPtr context = ipcContextCreate(3);

    Message msg = (Message) {.s_header.s_magic = 555, .s_header.s_payload_len = 55};
    int ret = ipcContextSend(context, 0, 2, &msg);
    assert(ret != -1);
    Message recv_msg;
    ret = ipcContextReceive(context, 0, 2, &recv_msg);
    assert(ret != -1);
    assert(recv_msg.s_header.s_magic == 555);
    assert(recv_msg.s_header.s_payload_len == 55);

    ipcContextDestroy(context);
}

void IpcContextTest_ReadFromEmptyPipe_ReturnZero() {
    IpcContextPtr context = ipcContextCreate(3);

    Message recv_msg;
    int ret = ipcContextReceive(context, 0, 2, &recv_msg);
    assert(ret == 0);

    ipcContextDestroy(context);
}

void IpcContextTest_WriteToOpenedPipe_ReturnMsgSize() {
    IpcContextPtr context = ipcContextCreate(3);

    Message msg;
    msg.s_header.s_payload_len = 0;
    int ret = ipcContextSend(context, 0, 2, &msg);
    assert(ret == sizeof(MessageHeader) + msg.s_header.s_payload_len);

    ipcContextDestroy(context);
}

void IpcContextTest_WriteToClosedPipe_ReturnNegVal() {
    IpcContextPtr context = ipcContextCreate(3);

    Message msg;
    ipcContextPrepare(context, 2);
    int ret = ipcContextSend(context, 0, 2, &msg);
    assert(ret == -1);

    ipcContextDestroy(context);
}
