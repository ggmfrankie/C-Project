//
// Created by Stefan on 23.03.2026.
//

#include "Time.hpp"

namespace ggm::Time {
    Timestamp getTimestamp() {
        using namespace std::chrono;
        Timestamp ts{};
        const auto now = system_clock::now();

        const auto dp = time_point_cast<days>(now);
        const year_month_day ymd{dp};

        const auto tod = now - dp;
        const hh_mm_ss hms{tod};

        ts.year  =  static_cast<short>(static_cast<int>(ymd.year()));
        ts.month = static_cast<Month>(static_cast<unsigned>(ymd.month()));
        ts.day   = static_cast<short>(static_cast<unsigned>(ymd.day()));

        auto&[hour, minute, second] = ts.dayTime;
        hour   = static_cast<short>(hms.hours().count());
        minute = static_cast<short>(hms.minutes().count());
        second = static_cast<short>(hms.seconds().count());

        return ts;
    }

    double getNow() {

    }
}


