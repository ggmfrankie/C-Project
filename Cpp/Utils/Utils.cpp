//
// Created by ertls on 18.02.2026.
//

#include "Utils.h"



namespace Utils {


    std::vector<std::string_view> split(std::string_view s, char delim) {
        std::vector<std::string_view> out;

        for (auto&& part : s | std::views::split(delim)) {
            auto sv = std::string_view(part.begin(), std::ranges::distance(part));
            out.push_back(sv);
        }
        return out;
    }
}
