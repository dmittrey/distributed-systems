#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "channel.h"

#define READ_IDX 0
#define WRITE_IDX 1

void channelCreate(ChannelPtr ptr) {
    pipe(ptr->fd);
    fcntl(ptr->fd[0], F_SETFL, fcntl(ptr->fd[0], F_GETFL) | O_NONBLOCK);
    fcntl(ptr->fd[1], F_SETFL, fcntl(ptr->fd[1], F_GETFL) | O_NONBLOCK);
}
void channelDestroy(ChannelPtr instance) {
    close(instance->fd[0]);
    close(instance->fd[1]);
}

void channelCloseI(ChannelPtr instance) {
    close(instance->fd[0]);
}
void channelCloseO(ChannelPtr instance) {
    close(instance->fd[1]);
}
void channelCloseIO(ChannelPtr instance) {
    close(instance->fd[0]);
    close(instance->fd[1]);
}

int channelWrite(const ChannelPtr instance, const Message *const msg) {
    return write(instance->fd[WRITE_IDX], msg, sizeof(MessageHeader) + msg->s_header.s_payload_len);
}
int channelRead(const ChannelPtr instance, Message *const msg) {
    int ret = read(instance->fd[READ_IDX], &msg->s_header, sizeof(MessageHeader));
    if (ret == -1) {
        if (errno == EAGAIN)
            return 0;
        else 
            return -1;
    }
    if (ret == sizeof(MessageHeader))
        ret += read(instance->fd[READ_IDX], msg->s_payload, msg->s_header.s_payload_len);
    return ret;
}
