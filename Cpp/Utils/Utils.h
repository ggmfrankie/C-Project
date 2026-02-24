//
// Created by ertls on 18.02.2026.
//

#ifndef MIXEDPROJECT_UTILS_H
#define MIXEDPROJECT_UTILS_H
#include <string>
#include <vector>
#include <ranges>

namespace Utils {
    std::vector<std::string_view> split(std::string_view s, char delim);

    struct to_vector_fn {
        template <std::ranges::input_range R>
        auto operator()(R&& r) const {
            using T = std::ranges::range_value_t<R>;
            return std::vector<T>(std::ranges::begin(r), std::ranges::end(r));
        }

        // Enable piping:  (range | to_vector)
        template <std::ranges::input_range R>
        friend auto operator|(R&& r, const to_vector_fn& f) {
            return f(std::forward<R>(r));
        }
    };

    inline constexpr to_vector_fn to_vector;

    double getDouble(const char *start, size_t length);
}

#endif //MIXEDPROJECT_UTILS_H