//
// Created by Stefan on 07.11.2025.
//

#include "Time.h"

#include <stdint.h>
#include <time.h>

#include "Utils/DataStructures/CString.h"

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#else
#include <bits/types/clockid_t.h>
#endif

TimeNs Time_nowNs() {
#ifdef _WIN32
    static LARGE_INTEGER freq;
    static int initialized = 0;
    if (!initialized) {
        QueryPerformanceFrequency(&freq);
        initialized = 1;
    }
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return (counter.QuadPart * 1000000000LL) / freq.QuadPart;

#else
    constexpr clockid_t clock = CLOCK_MONOTONIC;
    struct timespec time = {};
    clock_gettime(clock, &time);
    return (time.tv_sec * 1000000000LL) + time.tv_nsec;

#endif
}

const char* Time_getCurrentTimestamp() {
    static char buffer[80];

    time_t rawTime;
    time(&rawTime);
    const struct tm* timeInfo = localtime(&rawTime);

    strftime(buffer, sizeof(buffer), "%c", timeInfo);

    return buffer;
}
