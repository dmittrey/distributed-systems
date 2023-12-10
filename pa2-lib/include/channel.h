#pragma once

#include "ipc.h"

typedef struct Channel *ChannelPtr;

struct Channel {
    /* fd[0] - read end
       fd[1] - write end */
    int fd[2];
};

void channelCreate(ChannelPtr ptr);
void channelDestroy(ChannelPtr instance);

void channelCloseI(ChannelPtr instance);
void channelCloseO(ChannelPtr instance);
void channelCloseIO(ChannelPtr instance);

/*
-1          : On error
0 or above  : On success
*/
int channelWrite(const ChannelPtr instance, const Message *const msg);
/*
-1          : On error
0 or above  : Payload size in msg
*/
int channelRead(const ChannelPtr instance, Message *const msg);
