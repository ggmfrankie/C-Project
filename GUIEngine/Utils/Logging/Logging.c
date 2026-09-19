//
// Created by ertls on 17.09.2026.
//
#include "Logging.h"

#include <stdarg.h>

#include "Utils/Os/Time.h"

void _Log_log(int indent, const char* level, const char* file, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    size_t size = snprintf(nullptr, 0, "%*s%s [%s] %-16s %s\n", indent, " ", Time_getCurrentTimestamp(), level, file, fmt);

    char buff[size+1];
    snprintf(buff, sizeof(buff), "%*s%s [%s] %-16s %s\n", indent, " ", Time_getCurrentTimestamp(), level, file, fmt);
    vfprintf(LOG_OUTPUT_STREAM, buff, args);
    va_end(args);
}
