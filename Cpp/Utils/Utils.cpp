//
// Created by ertls on 18.02.2026.
//

#include "Utils.h"



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

    float getFloat(char* start, size_t length) {}
}
