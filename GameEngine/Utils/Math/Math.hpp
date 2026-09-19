//
// Created by ertls on 27.08.2026.
//

#pragma once
#include <numbers>

constexpr float operator""_deg(long double deg) {
    return static_cast<float>(deg * 3.14159265358979323846L / 180.0L);
}

namespace ggm {
    constexpr float toRad(const float degree) {
        return degree * (static_cast<float>(std::numbers::pi) / 180);
    }
}
