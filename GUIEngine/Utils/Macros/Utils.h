//
// Created by Stefan on 07.11.2025.
//

#pragma once
#ifndef __cplusplus
#include "GuiDefines.h"
#include "Macros.h"
#define repeat_for(num, ...)\
    for(int __i = 0; __i < (num); __i++){\
        __VA_ARGS__;\
    }
#define sig(x) (((x) > 0) - ((x) < 0))

#define clamp_impl(_x, _lo, _hi, x, lo, hi) ({\
    typeof(x) _x = (x);\
    typeof(lo) _lo = (lo);\
    typeof(hi) _hi = (hi);\
    _x < _lo ? _lo : (_x > _hi ? _hi : _x);\
})
#define clamp(x, lo, hi) clamp_impl(CONCAT(_x, __COUNTER__), CONCAT(_lo, __COUNTER__), CONCAT(_hi, __COUNTER__), x, lo, hi)

#define min_impl(_a, _b, a, b) ({typeof(a) _a = (a); typeof(b) _b = (b); _a < _b ? _a : _b;})
#define min(a, b) min_impl(CONCAT(_a, __COUNTER__), CONCAT(_b, __COUNTER__), (a), (b))
//#define min(a, b) (((a) < (b)) ? (a) : (b))

#define max_impl(_a, _b, a, b) ({typeof(a) _a = (a); typeof(b) _b = (b); _a > _b ? _a : _b;})
#define max(a, b) max_impl(CONCAT(_a, __COUNTER__), CONCAT(_b, __COUNTER__), (a), (b))
//#define max(a, b) (((a) > (b)) ? (a) : (b))

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
