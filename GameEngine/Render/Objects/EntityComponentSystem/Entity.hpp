//
// Created by Stefan on 11.10.2026.
//

#pragma once
#include <functional>
#include "ComponentRegistry.hpp"
#include "ggmdef.hpp"

namespace ggm {
    struct Entity {
        u32 mId;
        u32 mGeneration;
        u32 mArchetypeId;

        Entity();
        ~Entity();

        auto operator<=>(const Entity&) const = default;

        friend struct std::hash<Entity>;
    };
} // ggm

template<>
struct std::hash<ggm::Entity> {
    std::size_t operator()(const ggm::Entity& o) const noexcept {
        return std::hash<int>{}(o.mId);
    }
};
