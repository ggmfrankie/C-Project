//
// Created by Stefan on 11.10.2026.
//

#include "Entity.hpp"

#include <bitset>

namespace ggm {
    const Entity Entity::Invalid{1};

    Entity::Entity()
    {
        static i64 generator = 0;
        mId = generator++;
    }

    constexpr Entity::Entity(i64 index)
    {
        mId = index;
    }

    Entity::~Entity() {}


} // ggm