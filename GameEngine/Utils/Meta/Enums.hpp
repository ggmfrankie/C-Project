//
// Created by Stefan on 27.11.2026.
//

#pragma once
#include <utility>
#include "DataStorage/HashMap.hpp"
#include "DataStorage/StringView.hpp"

namespace ggm {
    class Enums {
        static constexpr i64 LIMIT_MIN = -128;
        static constexpr i64 LIMIT_MAX = 256;

        template<auto V>
        struct Print;

        /**
         * @brief StringView ggm::Enums::getName() [V = ggm::Enums::Test::ONE]
         */
        template<auto V>
        static constexpr StringView getExpression() {
            return __PRETTY_FUNCTION__;
        }

        /**
        * @brief StringView ggm::Enums::getName() [V = main()::Vallah::ONE]
                 StringView ggm::Enums::getName() [V = (Vallah)1]
         */
        static consteval std::optional<StringView> extractEnumName(StringView sv) {
            const auto last = sv.end()-1;
            for (auto it = last; it > sv.begin(); --it) {
                if (*it == ')') return std::nullopt;
                if (*it == ':') return StringView{it+1, last};
            }
            throw "Parsing Error";
        }

        template<typename T>
            requires std::is_enum_v<T>
        static consteval auto generateNames() {
            return [] {
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
        }

    public:
        template<typename T>
            requires std::is_enum_v<T>
        static constexpr StringView name(T value) {
            static auto mappings = [&] {
                // TODO: use lookup table
                HashMap<T, StringView> out {16};
                for (auto& pair : generateNames<T>()) {
                    if (pair) out.insert(pair->first, pair->second);
                }
                return out;
            }();
            auto result = mappings[value];
            return result ? *result : "Invalid";
        }
    };
}

template<typename T>
requires std::is_enum_v<T>
std::ostream& operator<<(std::ostream& os, T value) {
    return os << ggm::Enums::name(value);
}
