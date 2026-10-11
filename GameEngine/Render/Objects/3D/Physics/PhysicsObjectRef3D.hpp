//
// Created by Stefan on 15.03.2026.
//

#pragma once
#include "../EngineDefines.hpp"
#include <Jolt/Jolt.h>
#include "Jolt/Physics/Body/BodyID.h"
#include "Jolt/Physics/Body/BodyInterface.h"
#include "Utils/Math/Quaternion.hpp"
#include "Utils/Math/Vector.hpp"

namespace ggm {
    class PhysicsObjectRef3D {
        JPH::BodyID mBodyID;
        JPH::BodyInterface& mBodyInterface;

    public:
        PhysicsObjectRef3D(JPH::BodyID ID, JPH::BodyInterface& bodyInterface);

        [[nodiscard]] JPH::BodyID getBodyID() const { return mBodyID; }

        void applyForce(float x, float y, float z) const;
        void setPosition(float x, float y, float z);

        void setRotation(float pitch, float yaw, float roll);

        [[nodiscard]] ggm::Vector3f getPosition() const;

        [[nodiscard]] ggm::Quaternion getRotation() const;
    };
} // Obj
