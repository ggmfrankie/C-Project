//
// Created by Stefan on 18.03.2026.
//

#pragma once
#include "GameEngine.hpp"
#include "Jolt/Physics/PhysicsSystem.h"

namespace Game {
    class Player : public Obj::GameObject{
        Player();

        Player(ggm::Vector3f &pos, JPH::PhysicsSystem &system);

        ~Player();

    };
} // Game