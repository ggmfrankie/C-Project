//
// Created by Stefan on 11.10.2026.
//

#include "Entity.hpp"

#include <bitset>
#include <iostream>

namespace ggm {
    const Entity Entity::Invalid{-1};

    i64 Entity::id() const {
        return mId;
    }

    Entity::Entity():
        mId([] {static i64 generator = 0; return generator++;}())
    {}

    constexpr Entity::Entity(i64 index):
        mId(index)
    {}

    Entity::~Entity() {}
} // ggm