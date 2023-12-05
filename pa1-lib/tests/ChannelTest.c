#include <assert.h>

#include "channel.h"

void ChannelTest_ReadFromEmptyChannel_ReturnNegOne() {
    Message msg;
    struct Channel instance;
    channelCreate(&instance);
    int ret = channelRead(&instance, &msg);
    assert(ret == 0);
}

void ChannelTest_ReadFromOnlyHeaderMessage_ReturnHeaderSize() {
    Message msg;
    msg.s_header.s_payload_len = 0;
    struct Channel instance;
    channelCreate(&instance);
    channelWrite(&instance, &msg);
    int ret = channelRead(&instance, &msg);
    assert(ret == sizeof(MessageHeader));
}

void ChannelTest_ReadFromFilledMessage_ReturnPayloadAndHeaderSize() {
    Message msg;
    msg.s_header.s_payload_len = 50;
    struct Channel instance;
    channelCreate(&instance);
    channelWrite(&instance, &msg);
    int ret = channelRead(&instance, &msg);
    assert(ret == sizeof(MessageHeader) + 50);
}

void ChannelTest_ReadTwoMessages_ReturnPayloadAndHeaderSizeTwice() {
    Message msg;
    struct Channel instance;
    channelCreate(&instance);
    msg.s_header.s_payload_len = 50;
    channelWrite(&instance, &msg);
    msg.s_header.s_payload_len = 80;
    channelWrite(&instance, &msg);
    assert(channelRead(&instance, &msg) == sizeof(MessageHeader) + 50);
    assert(channelRead(&instance, &msg) == sizeof(MessageHeader) + 80);
}

void ChannelTest_WriteHeaderMessage_ReturnHeaderSize() {
    Message msg;
    msg.s_header.s_payload_len = 0;
    struct Channel instance;
    channelCreate(&instance);
    int ret = channelWrite(&instance, &msg);
    assert(ret == sizeof(MessageHeader));
}

void ChannelTest_WriteFilledMessage_ReturnPayloadSize() {
    Message msg;
    msg.s_header.s_payload_len = 50;
    struct Channel instance;
    channelCreate(&instance);
    int ret = channelWrite(&instance, &msg);
    assert(ret == sizeof(MessageHeader) + msg.s_header.s_payload_len);
}
