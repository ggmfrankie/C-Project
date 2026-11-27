//
// Created by Stefan on 27.11.2026.
//

#pragma once
#include <utility>
#include "DataStorage/HashMap.hpp"
#include "DataStorage/StringView.hpp"

namespace ggm::Enums {
    enum class Test { ONE, TWO };

    constexpr i64 LIMIT_MIN = -128;
    constexpr i64 LIMIT_MAX = 256;

    template<auto V>
    struct Print;

    /**
     * @brief StringView ggm::Enums::getName() [V = ggm::Enums::Test::ONE]
     */
    template<auto V>
    constexpr StringView getExpression() {
        return __PRETTY_FUNCTION__;
    }

    /**
    * @brief StringView ggm::Enums::getName() [V = main()::Vallah::ONE]
             StringView ggm::Enums::getName() [V = (Vallah)1]
     */
    consteval std::optional<StringView> extractEnumName(StringView sv) {
        const auto last = sv.end()-1;
        for (auto it = last; it > sv.begin(); --it) {
            if (*it == ')') return std::nullopt;
            if (*it == ':') return StringView{it+1, last};
        }
        throw "Parsing Error";
    }

    template<typename T>
        requires std::is_enum_v<T>
    consteval auto generateNames() {
        static constexpr auto mappings = [] {
            std::array<std::optional<std::pair<T, StringView>>, LIMIT_MAX - LIMIT_MIN> matches;
            u64 index = 0;

            [&]<u64... values>(std::index_sequence<values...>) {
                (
                    [&] {
                        constexpr i64 val = values + LIMIT_MIN;

                        constexpr std::optional<StringView> value = extractEnumName(getExpression<static_cast<T>(val)>());

                        if constexpr (value) matches[index++] = std::pair{static_cast<T>(val), *value};
                    }(), ...
                );
            }(std::make_index_sequence<LIMIT_MAX - LIMIT_MIN>{});
            return matches;
        }();

        u64 enumSize = 0;
        for (auto& mapping : mappings) {
            if (mapping) enumSize++;
        }

        return std::span{mappings.data(), enumSize};
    }

    template<typename Map, typename T>
    StringView getName(const Map& mappings, T val) {
        for (auto& mapping : mappings) {
            if (mapping->first == val) return mapping->second;
        }
        return "Unknown";
    }

    template<typename T>
        requires std::is_enum_v<T>
    constexpr StringView name(T value) {
        constexpr auto map = generateNames<T>();
        return getName(map, value);
    }
}
