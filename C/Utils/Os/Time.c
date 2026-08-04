//
// Created by Stefan on 07.11.2025.
//

#include "Time.h"

#include <stdint.h>
#include <time.h>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#else
#include <bits/types/clockid_t.h>
#endif

TimeNs now_ns() {
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

uint64_t now_ns_wallclock() {
#ifdef _WIN32
    FILETIME ft;
    GetSystemTimePreciseAsFileTime(&ft);

    ULARGE_INTEGER u;
    u.LowPart  = ft.dwLowDateTime;
    u.HighPart = ft.dwHighDateTime;

    // FILETIME is 100 ns intervals since Jan 1, 1601 (UTC)
    return u.QuadPart * 100;
#else

#endif
}
