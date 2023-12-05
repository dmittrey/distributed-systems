#include <assert.h>
#include <unistd.h>
#include <stdlib.h>

#include "ipc.h"
#include "context.h"

void IpcTest_SendBetweenProcesses_Sucessful() {
    IpcContextPtr ipcContext = ipcContextCreate(3);
    ContextPtr ctx_0 = contextCreate(0, ipcContext);
    ContextPtr ctx_2 = contextCreate(2, ipcContext);

    Message msg;
    msg.s_header.s_magic = 555;
    msg.s_header.s_payload_len = 55;
    int ret = send(ctx_0, 2, &msg);
    assert(ret == 0);
    Message recv_msg;
    recv_msg.s_header.s_magic = 0;
    recv_msg.s_header.s_payload_len = 0;
    ret = receive(ctx_2, 0, &recv_msg);
    assert(ret == 0);
    assert(recv_msg.s_header.s_magic == 555);
    assert(recv_msg.s_header.s_payload_len == 55);

    ipcContextDestroy(ipcContext);
    contextDestroy(ctx_0);
    contextDestroy(ctx_2);
}

void IpcTest_SendMulticast_Successful() {
    IpcContextPtr ipcContext = ipcContextCreate(3);
    ContextPtr ctx_0 = contextCreate(0, ipcContext);
    ContextPtr ctx_1 = contextCreate(1, ipcContext);
    ContextPtr ctx_2 = contextCreate(2, ipcContext);

    Message msg;
    msg.s_header.s_magic = 5;
    msg.s_header.s_payload_len = 10;
    int ret = send_multicast(ctx_0, &msg);
    assert(ret == 0);

    Message recv_msg;
    recv_msg.s_header.s_magic = 0;
    recv_msg.s_header.s_payload_len = 0;
    ret = receive(ctx_1, 0, &recv_msg);
    assert(ret == 0);
    assert(recv_msg.s_header.s_magic == 5);
    assert(recv_msg.s_header.s_payload_len == 10);

    recv_msg.s_header.s_magic = 0;
    recv_msg.s_header.s_payload_len = 0;
    ret = receive(ctx_2, 0, &recv_msg);
    assert(ret == 0);
    assert(recv_msg.s_header.s_magic == 5);
    assert(recv_msg.s_header.s_payload_len == 10);
    
    ipcContextDestroy(ipcContext);
    contextDestroy(ctx_0);
    contextDestroy(ctx_1);
    contextDestroy(ctx_2);
}

void IpcTest_ReceiveAny_Successful() {
    IpcContextPtr ipcContext = ipcContextCreate(3);
    ContextPtr ctx_1 = contextCreate(1, ipcContext);
    ContextPtr ctx_2 = contextCreate(2, ipcContext);

    Message msg;
    msg.s_header.s_magic = 5;
    msg.s_header.s_payload_len = 10;
    int ret = send(ctx_1, 2, &msg);
    assert(ret == 0);

    Message recv_msg;
    recv_msg.s_header.s_magic = 0;
    recv_msg.s_header.s_payload_len = 0;
    ret = receive_any(ctx_2, &recv_msg);
    assert(ret == 0);
    assert(recv_msg.s_header.s_magic == 5);
    assert(recv_msg.s_header.s_payload_len == 10);

    ipcContextDestroy(ipcContext);
    contextDestroy(ctx_1);
    contextDestroy(ctx_2);
}

/* 2 before 1 */
void IpcTest_ReceiveAnyToHostTwice_Sucessful() {
    IpcContextPtr ipcContext = ipcContextCreate(3);
    ContextPtr ctx_0 = contextCreate(0, ipcContext);
    ContextPtr ctx_1 = contextCreate(1, ipcContext);
    ContextPtr ctx_2 = contextCreate(2, ipcContext);

    Message msg;
    msg.s_header.s_magic = 5;
    msg.s_header.s_payload_len = 0;

    int ret = fork();
    if (!ret) {
        assert(send(ctx_1, 0, &msg) == 0);
        exit(1);
    }
    ret = fork();
    if (!ret) {
        sleep(2);
        assert(send(ctx_2, 0, &msg) == 0);
        exit(1);
    }

    ret = receive_any(ctx_0, &msg);
    assert(ret == 0);
    ret = receive_any(ctx_0, &msg);
    assert(ret == 0);
    assert(msg.s_header.s_magic == 5);
    assert(msg.s_header.s_payload_len == 0);
    for (int i = 0; i < 2; i++)
        wait(NULL);
}

void IpcTest_ReceiveAnyToHostTwice2_Sucessful() {
    IpcContextPtr ipcContext = ipcContextCreate(3);
    ContextPtr ctx_0 = contextCreate(0, ipcContext);
    ContextPtr ctx_1 = contextCreate(1, ipcContext);
    ContextPtr ctx_2 = contextCreate(2, ipcContext);

    Message msg;
    msg.s_header.s_magic = 5;
    msg.s_header.s_payload_len = 0;

    int ret = fork();
    if (!ret) {
        // sleep(2);
        assert(send(ctx_1, 0, &msg) == 0);
        exit(1);
    }
    ret = fork();
    if (!ret) {
        assert(send(ctx_2, 0, &msg) == 0);
        exit(1);
    }

    ret = receive_any(ctx_0, &msg);
    assert(ret == 0);
    ret = receive_any(ctx_0, &msg);
    assert(ret == 0);
    assert(msg.s_header.s_magic == 5);
    assert(msg.s_header.s_payload_len == 0);
    for (int i = 0; i < 2; i++)
        wait(NULL);
}

void IpcTest_ReceiveAnyToHostTwice3_Sucessful() {
    IpcContextPtr ipcContext = ipcContextCreate(3);
    ContextPtr ctx_0 = contextCreate(0, ipcContext);
    ContextPtr ctx_1 = contextCreate(1, ipcContext);

    Message msg;
    msg.s_header.s_magic = 5;
    msg.s_header.s_payload_len = 0;

    int ret = fork();
    if (!ret) {
        sleep(2);
        assert(send(ctx_1, 0, &msg) == 0);
        exit(1);
    }
    ret = fork();
    if (!ret) {
        assert(send(ctx_1, 0, &msg) == 0);
        exit(1);
    }

    ret = receive_any(ctx_0, &msg);
    assert(ret == 0);
    ret = receive_any(ctx_0, &msg);
    assert(ret == 0);
    assert(msg.s_header.s_magic == 5);
    assert(msg.s_header.s_payload_len == 0);
    for (int i = 0; i < 2; i++)
        wait(NULL);
}
