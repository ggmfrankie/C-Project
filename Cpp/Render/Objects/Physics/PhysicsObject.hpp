//
// Created by Stefan on 15.03.2026.
//

#pragma once
#include <Jolt/Jolt.h>
#include "Jolt/Physics/Body/BodyID.h"
#include "Jolt/Physics/Body/BodyInterface.h"
#include "Utils/Math/Quaternion.hpp"
#include "Utils/Math/Vector.hpp"

namespace Obj {
    class PhysicsObject {
        JPH::BodyID mBodyID;
        JPH::BodyInterface& mBodyInterface;

    public:
        PhysicsObject(JPH::BodyID ID, JPH::BodyInterface& bodyInterface);

        JPH::BodyID getBodyID() const { return mBodyID; }

        void applyForce(float x, float y, float z) const;
        void setPosition(float x, float y, float z) const;

        ggm::Vector3f getPosition() const;

        ggm::Quaternion getRotation() const;
    };
} // Obj
