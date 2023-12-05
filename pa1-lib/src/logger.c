#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "logger.h"

LoggerPtr loggerCreate(const char *const file_path) {
    LoggerPtr instance = (LoggerPtr) malloc(sizeof(struct Logger));
    instance->fd = open(file_path, O_CREAT | O_APPEND | O_WRONLY , 0644);
    return instance;
}
void loggerDestroy(LoggerPtr instance) {
    close(instance->fd);
    free(instance);
}
