//
// Created by Stefan on 11.10.2026.
//

#include "Entity.hpp"

#include <bitset>

namespace ggm {
    Entity::Entity()
    {
        static u64 generator = 0;
        mId = generator++;
    }

    Entity::~Entity() {}
} // ggm