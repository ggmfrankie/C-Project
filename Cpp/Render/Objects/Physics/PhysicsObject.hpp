//
// Created by Stefan on 15.03.2026.
//

#pragma once
#define JPH_PLATFORM_WINDOWS
#define JPH_CPU_X64
#define JPH_COMPILER_GCC
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

        PhysicsObject(const PhysicsObject&) = delete;
        PhysicsObject& operator=(const PhysicsObject&) = delete;

        PhysicsObject(const PhysicsObject&&) noexcept;

        [[nodiscard]] JPH::BodyID getBodyID() const { return mBodyID; }

        void applyForce(float x, float y, float z) const;
        void setPosition(float x, float y, float z);

        void setRotation(float pitch, float yaw, float roll);

        [[nodiscard]] ggm::Vector3f getPosition() const;

        [[nodiscard]] ggm::Quaternion getRotation() const;
    };
} // Obj
