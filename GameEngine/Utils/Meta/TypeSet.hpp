//
// Created by Stefan on 16.11.2026.
//

#pragma once
#include <concepts>

namespace ggm {
    template<typename... Ts>
    struct AllUniqueImpl;

    template<typename T, typename... Ts>
    struct AllUniqueImpl<T, Ts...> {
        static constexpr bool value = (!std::same_as<T, Ts> && ...) && AllUniqueImpl<Ts...>::value;
    };

    template<>
    struct AllUniqueImpl<> {
        static constexpr bool value = true;
    };

    template<std::size_t index, typename Curr, typename... Ts>
    struct GetTypeAtImpl {
        using type = GetTypeAtImpl<index-1, Ts...>::type;
    };

    template<typename Curr, typename... Ts>
    struct GetTypeAtImpl<0, Curr, Ts...> {
        using type = Curr;
    };

    template<std::size_t index, typename... Ts>
    using GetTypeAt = GetTypeAtImpl<index, Ts...>::type;

    template<typename... Ts>
    constexpr bool AllUnique = [] consteval {
        static_assert(AllUniqueImpl<Ts...>::value, "Type occurs more than once in type pack");
        return true;
    }();

    template<typename... Ts>
        requires AllUnique<Ts...>
    struct TypeSet {

        template<typename T>
        static consteval u64 componentId() {
            constexpr bool matches[] = {
                std::is_same_v<T, Ts>...
            };

            for (u64 i = 0; i < sizeof...(Ts); ++i) {
                if (matches[i]) return i;
            }

            throw "Component not found";
        }

        static constexpr std::size_t size = sizeof...(Ts);

        static constexpr bool empty = false;

        template<typename Searched>
        static constexpr bool contains = (std::same_as<std::remove_cvref_t<Searched>, Ts> || ...);

        template<typename Searched>
        static constexpr u64 index = componentId<std::remove_cvref_t<Searched>>();

        template<std::size_t index>
        using at = GetTypeAt<index, Ts...>;

        template<typename Inserted>
        using insert = TypeSet<Ts..., Inserted>;
    };

    template<>
    struct TypeSet<> {
        static constexpr std::size_t size = 0;

        static constexpr bool empty = true;

        template<typename Searched>
        static constexpr bool contains = false;

        template<typename Inserted>
        using insert = TypeSet<Inserted>;
    };

    template<typename... Ts>
    using CleanTypeSet = TypeSet<std::remove_cvref_t<Ts>...>;
}
