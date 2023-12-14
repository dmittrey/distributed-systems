#pragma once

#include "logger.h"
#include "channel.h"

void loggerChannelIClosed(LoggerPtr instance, local_id id, ChannelPtr channel);
void loggerChannelOClosed(LoggerPtr instance, local_id id, ChannelPtr channel);
void loggerChannelIOClosed(LoggerPtr instance, local_id id, ChannelPtr channel);
