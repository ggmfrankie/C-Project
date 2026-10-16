//
// Created by Stefan on 11.10.2026.
//

#include "Entity.hpp"

#include <bitset>

namespace ggm {
    Entity::Entity() :
        mGeneration(0),
        mArchetypeId(0)
    {
        static u64 generator = 0;
        mId = generator++;

        std::bitset<5> ad{};
    }

    Entity::~Entity() {}
} // ggm