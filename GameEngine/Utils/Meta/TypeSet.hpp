//
// Created by Stefan on 16.11.2026.
//

#pragma once
#include <concepts>

namespace ggm {
    template<typename... Ts>
    struct AllUnique;

    template<typename T, typename... Ts>
    struct AllUnique<T, Ts...> {
        static constexpr bool value = (!std::same_as<T, Ts> && ...) && AllUnique<Ts...>::value;
    };

    template<>
    struct AllUnique<> {
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
    constexpr bool allUnique = AllUnique<Ts...>::value;

    template<typename... Ts>
        requires allUnique<Ts...>
    struct TypeSet {
        static constexpr std::size_t size = sizeof...(Ts);

        static constexpr bool empty = false;

        template<typename Searched>
        static constexpr bool contains = (std::same_as<Searched, Ts> || ...);

        template<u64 index>
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
