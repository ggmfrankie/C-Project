//
// Created by Stefan on 18.03.2026.
//

#pragma once
#include "GameEngine.hpp"
#include "Jolt/Physics/PhysicsSystem.h"
#include "Jolt/Physics/Character/Character.h"
#include "Render/Objects/Render/RenderObject3D.hpp"

namespace Game {
    class Player {
        Obj3D::RenderObject3D mRenderObj;
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