//
// Created by Stefan on 18.03.2026.
//

#pragma once
#include "GameEngine.hpp"
#include "Jolt/Physics/PhysicsSystem.h"
#include "Jolt/Physics/Character/Character.h"

namespace Game {
    class Player {
        Obj::RenderObject mRenderObj;
        ggm::Vector3f mPos{};

        JPH::Ref<JPH::Character> mCharacter;
    public:
        Player(const ggm::Vector3f &pos, JPH::PhysicsSystem &system);

        Player(Player &&other) noexcept;

        Player();

        void init() const;

        ~Player();

        Player &operator=(Player &&other) noexcept;

    };
} // Game