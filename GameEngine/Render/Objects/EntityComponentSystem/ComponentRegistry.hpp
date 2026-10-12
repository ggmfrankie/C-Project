//
// Created by Stefan on 12.10.2026.
//

#pragma once
#include <variant>

#include "Object.hpp"
#include "DataStorage/HashMap.hpp"
#include "Utils/Math/Vector.h"

namespace ggm {

    struct Sprite {
        Vec2f dims;
    };

    struct Position: Vec2f {
        using Vec2f::Vec2f;
    };

    struct Velocity: Vec2f {
        using Vec2f::Vec2f;
    };

    struct Health {
        int amount;
    };




    template<typename... Components>
    class ComponentRegistry {
    public:
        using Component = std::variant<std::vector<Components>...>;

        struct BaseArcheType {
            virtual ~BaseArcheType() = default;
        };

        struct Archetype : BaseArcheType {
            std::vector<Component> components;
            std::array<u16, sizeof...(Components)> mapping;

            Archetype() {
                (components.emplace_back(
                    std::in_place_type<std::vector<UsedComponents>>
                ),...);
            }
        };

    private:
        std::vector<std::unique_ptr<BaseArcheType>> mArchetypes;

    };
} // ggm
