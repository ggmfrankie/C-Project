//
// Created by ertls on 18.02.2026.
//

#pragma once
#include <vector>
#include <ranges>

namespace ggm {
    std::vector<std::string_view> split(std::string_view s, char delim);
    double getDouble(const char *start, size_t length);

}

constexpr float operator""_deg(long double deg) {
    return static_cast<float>(deg * 3.14159265358979323846L / 180.0L);
}
