//
// Created by Stefan on 07.11.2025.
//

#include "TimeMeasurenments.h"

#include <stdint.h>
#include <windows.h>

TimeNs now_ns() {
    static LARGE_INTEGER freq;
    static int initialized = 0;
    if (!initialized) {
        QueryPerformanceFrequency(&freq);
        initialized = 1;
    }
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return (counter.QuadPart * 1000000000LL) / freq.QuadPart;
}

uint64_t now_ns_wallclock(void)
{
    FILETIME ft;
    GetSystemTimePreciseAsFileTime(&ft);

    ULARGE_INTEGER u;
    u.LowPart  = ft.dwLowDateTime;
    u.HighPart = ft.dwHighDateTime;

    // FILETIME is 100 ns intervals since Jan 1, 1601 (UTC)
    return u.QuadPart * 100;
}
