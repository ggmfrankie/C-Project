//
// Created by Stefan on 11.10.2026.
//

#pragma once
#include <functional>
#include "ggmdef.hpp"

namespace ggm {
    struct Entity {
        Entity();
        ~Entity();

        auto operator<=>(const Entity&) const = default;
        friend struct std::hash<Entity>;

        static const Entity Invalid;

        i64 id() const;

    private:
        i64 mId = -1;
        explicit constexpr Entity(i64 index);
    };
} // ggm

template<>
struct std::hash<ggm::Entity> {
    std::size_t operator()(const ggm::Entity& o) const noexcept {
        return std::hash<int>{}(o.mId);
    }
};
