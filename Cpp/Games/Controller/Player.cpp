//
// Created by Stefan on 18.03.2026.
//

#include "Jolt/Jolt.h"
#include "Player.hpp"

#include "Games/DefaultLayer/PhysicsHandler.hpp"
#include "Jolt/Physics/Character/Character.h"
#include "Jolt/Physics/Character/Character.h"
#include "Jolt/Physics/Collision/Shape/Shape.h"
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>

namespace Game {

    Obj::RenderObject createPlayerRenderObject(){
        return Obj::RenderObject("grass_block\\grass_block.obj");
    }

    Player::Player(ggm::Vector3f& pos, JPH::PhysicsSystem& system) :
        mRenderObj(createPlayerRenderObject())
    {
        JPH::CharacterSettings settings{};
        settings.mMass = 10;
        settings.mLayer = Layers::MOVING;
        settings.mMaxSlopeAngle = JPH::DegreesToRadians(45.0f);
        settings.mShape = new JPH::CapsuleShape(0.5, .01);

        JPH::Ref character = new JPH::Character(&settings, {0, 0, 0}, {}, 0, &system);

        character->AddToPhysicsSystem();
    }

    Player::~Player() {
    }
} // Game