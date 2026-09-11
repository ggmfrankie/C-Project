//
// Created by Stefan on 07.11.2025.
//

#pragma once
#ifdef __cplusplus
extern "C" {
#endif

typedef long long TimeNs;
TimeNs now_ns();

#define measureTime(name, ...) do {\
    unsigned long long start = now_ns();\
    {\
        __VA_ARGS__    \
    }\
    printf("Elapsed time for %s: %llu\n", name, now_ns() - start);\
} while (0)

#ifdef __cplusplus
}
#endif
