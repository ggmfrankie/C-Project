//
// Created by ertls on 31.07.2026.
//

#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "../Macros/Utils.h"

#define ERROR_(fms, ...) do {fprintf(stderr, "[ERROR] File: " __FILE__ ":%i\n"fms"\n", __LINE__, ##__VA_ARGS__); exit(23);} while(0)
#define WARNING_(fms, ...) do {fprintf(stderr, "[WARNING] File: " __FILE__ ":%i\n"fms"\n", __LINE__, ##__VA_ARGS__);} while(0)
#define TODO_(fms, ...) do {fprintf(stderr, "[TODO] File: " __FILE__ ":%i\n"fms"\n", __LINE__, ##__VA_ARGS__); exit(21);} while(0)
#define INFO_(fms, ...) fprintf(stdout, "[INFO]: "fms"\n", ##__VA_ARGS__)

#define LOG_NONE 0
#define LOG_INFO 1
#define LOG_DEBUG 2
#define LOG_TRACE 3

#define LOG_LEVEL LOG_NONE

#define LOG_OUTPUT_STREAM stdout

void _Log_log(const char* level, const char* file, const char* fmt, ...);

#define Log_info(fmt, ...)  if(LOG_LEVEL >= LOG_INFO)  _Log_log("INFO",  __FILE_NAME__ ":" STRINGIFY(__LINE__), fmt, ##__VA_ARGS__)
#define Log_debug(fmt, ...) if(LOG_LEVEL >= LOG_DEBUG) _Log_log("DEBUG", __FILE_NAME__ ":" STRINGIFY(__LINE__), fmt, ##__VA_ARGS__)
#define Log_trace(fmt, ...) if(LOG_LEVEL >= LOG_TRACE) _Log_log("TRACE", __FILE_NAME__ ":" STRINGIFY(__LINE__), fmt, ##__VA_ARGS__)
#ifdef __cplusplus
}
#endif