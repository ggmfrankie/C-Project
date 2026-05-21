//
// Created by Stefan on 07.11.2025.
//

#pragma once
#ifndef __cplusplus
#include "GuiDefines.h"
#define repeat_for(num, ...)\
    for(int __i = 0; __i < (num); __i++){\
        __VA_ARGS__;\
    }
#define sig(x) (((x)<0)?-1:1)

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

#define print_if(condition, text, ...) do {if (condition) printf(text, ##__VA_ARGS__);} while(0)
#endif

#define only_every_do(num, ...) do {\
    static int count = 0;\
    if (count++ >= (num)) {\
        {\
            __VA_ARGS__;\
        }\
    count = 0;\
    }\
} while (0);

#define measure_time_of(name, ...) do {\
    unsigned long long start = now_ns();\
    {\
        __VA_ARGS__    \
    }\
    printf("Elapsed time for %s: %llu\n", name, now_ns() - start);\
} while (0)
