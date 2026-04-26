//
// Created by Stefan on 07.11.2025.
//

#pragma once
#include "GuiDefines.h"
#define REPEAT(num, ...)\
    for(int __i = 0; __i < (num); __i++){\
        __VA_ARGS__;\
    }
#define sig(x)\
    (((x)<0)?-1:((x)==0)?0:1)

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

#define only_every_do(num, ...) do {\
    static int count = 0;\
    if (count++ >= (num)) {\
        {\
            __VA_ARGS__;\
        }\
        count = 0;\
    }\
    \
} while (0);
