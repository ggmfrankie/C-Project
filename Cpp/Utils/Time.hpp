//
// Created by Stefan on 23.03.2026.
//

#pragma once
#include <chrono>

namespace ggm::Time {
        enum class Month {
            January = 1,
            February,
            March,
            April,
            May,
            June,
            Juli,
            August,
            September,
            October,
            November,
            December
        };

        enum class Day {
            Monday = 1,
            Tuesday,
            Wednesday,
            Thursday,
            Friday,
            Saturday,
            Sunday
        };

        struct DayTime {
            short hour;
            short minute;
            short second;
        };

        struct Timestamp {
            short year;
            Month month;
            short day;
            DayTime dayTime;
        };

        Timestamp getTimestamp();

}

