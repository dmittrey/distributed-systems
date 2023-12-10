#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>

#include "pipe.h"
#include "logger_channel.h"

void loggerChannelIClosed(LoggerPtr instance, local_id id, ChannelPtr channel) {
    dprintf(instance->fd, log_pipe_closed_i_fmt, id, channel->fd[0]);
}
void loggerChannelOClosed(LoggerPtr instance, local_id id, ChannelPtr channel) {
    dprintf(instance->fd, log_pipe_closed_o_fmt, id, channel->fd[1]);
}
void loggerChannelIOClosed(LoggerPtr instance, local_id id, ChannelPtr channel) {
    dprintf(instance->fd, log_pipe_closed_io_fmt, id, channel->fd[0], channel->fd[1]);
}
