//
// Created by Stefan on 11.10.2026.
//

#pragma once
#include <functional>
#include "ggmdef.hpp"

namespace ggm {
    struct Entity {
        i64 mId = -1;

        Entity();
        ~Entity();

        auto operator<=>(const Entity&) const = default;

        friend struct std::hash<Entity>;

    private:
        explicit constexpr Entity(i64 index);
    public:
        static const Entity Invalid;
    };
} // ggm

template<>
struct std::hash<ggm::Entity> {
    std::size_t operator()(const ggm::Entity& o) const noexcept {
        return std::hash<int>{}(o.mId);
    }
};
