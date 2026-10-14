//
// Created by Stefan on 11.10.2026.
//

#include "Entity.hpp"

namespace ggm {
    Entity::Entity() :
        mGeneration(0),
        mArchetypeId(0)
    {
        static u64 generator = 0;
        mId = generator++;
    }

    Entity::~Entity() {}
} // ggm