//
// Created by ertls on 18.02.2026.
//

#include "Utils.h"

#include "Math/ggmdef.h"
#include <math.h>

namespace Utils {

    using std::vector, std::string_view, std::ranges::distance;

    vector<string_view> split(string_view s, char delim) {
        vector<string_view> out;

        for (auto&& part : s | std::views::split(delim)) {
            auto sv = string_view(part.begin(), distance(part));
            out.push_back(sv);
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
                        result += (*next - '0') * pow(10, factor);
                        factor--;
                        state = State::Digit;
                    }
                    break;
            }
            next++;
        }
        return result * prefix;
    }
}
