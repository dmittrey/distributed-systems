#pragma once

typedef struct Logger *LoggerPtr;

struct Logger {
    int fd;
};

LoggerPtr loggerCreate(const char *const file_path);
void loggerDestroy(LoggerPtr instance);
