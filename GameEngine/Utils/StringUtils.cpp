//
// Created by ertls on 18.02.2026.
//

#include "StringUtils.hpp"

#include "ggmdef.hpp"
#include <cmath>
#include <iostream>

namespace ggm {

    using std::vector, std::string_view, std::ranges::distance;

    std::vector<std::string_view> split(std::string_view s, char delim) {
        std::vector<std::string_view> out;
        out.reserve(s.size() / 10);

        for (auto&& part : s | std::views::split(delim)) {
            const auto it = part.begin();
            if (it == part.end())
                continue;

            auto* ptr = &*it;

            size_t len = 0;
            for (auto jt = it; jt != part.end(); ++jt)
                ++len;

            out.emplace_back(ptr, len);
        }

        return out;
    }

    double getDouble(const char *start, const size_t length) {
        enum class State{
            WantSign,
            Digit
        };

        const char* next = start;
        const char* end = next + length;
        int factor = -1;

        if (*next == '-') --factor;
        for (; next < end; ++next, ++factor) {
            if (*next == '.') break;
        }
        next = start;

        auto state = State::WantSign;
        int prefix = 1;

        double result = 0.0;

        while (next < end) {
            switch (state) {
                case State::WantSign:
                    if (*next == '-') {
                        prefix = -1;
                    } else if (std::isdigit(*next)) {
                        goto Digit;
                    }
                    break;
                case State::Digit:
                    if (std::isdigit(*next)) {
                        Digit:
                        result += (*next - '0') * std::pow(10, factor);
                        --factor;
                        state = State::Digit;
                    }
                    break;
            }
            next++;
        }
        return result * prefix;
    }
}
