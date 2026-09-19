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
