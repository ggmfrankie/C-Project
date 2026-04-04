//
// Created by Stefan on 18.03.2026.
//

#include "Jolt/Jolt.h"
#include "Player.hpp"

#include "Games/DefaultLayer/PhysicsHandler3D.hpp"
#include "Jolt/Physics/Character/Character.h"
#include "Jolt/Physics/Character/Character.h"
#include "Jolt/Physics/Collision/Shape/Shape.h"
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>

namespace Game {

    Obj3D::RenderObject3D createPlayerRenderObject(){
        return Obj3D::RenderObject3D("grass_block\\grass_block.obj");
    }

    Player::Player(const ggm::Vector3f& pos, JPH::PhysicsSystem& system) :
        mRenderObj(createPlayerRenderObject()),
        mPos(pos)
    {
        JPH::Ref settings = new JPH::CharacterSettings{};
        settings->mMass = 10;
        settings->mLayer = Layers::MOVING;
        settings->mMaxSlopeAngle = JPH::DegreesToRadians(45.0f);
        settings->mShape = new JPH::CapsuleShape(0.5, .01);

        mCharacter = new JPH::Character(settings, {pos.x, pos.y, pos.z}, JPH::Quat::sIdentity(), 0, &system);
    }

    Player::Player(Player &&other) noexcept:
        mRenderObj(std::move(other.mRenderObj)),
        mPos(other.mPos),
        mCharacter(std::move(other.mCharacter))
    {
        other.mCharacter = nullptr;
    }

    Player::Player() :
        mRenderObj(),
        mPos(),
        mCharacter()
    {
    }

    void Player::init() const {
        mCharacter->AddToPhysicsSystem();
    }

    Player::~Player() {
        if (mCharacter != nullptr) mCharacter->RemoveFromPhysicsSystem();
    }

    Player& Player::operator=(Player&& other) noexcept
    {
        if (this != &other)
        {
            mPos = other.mPos;
            mRenderObj = std::move(other.mRenderObj);
        }
        return *this;
    }
} // Game