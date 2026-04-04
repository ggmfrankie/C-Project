//
// Created by Stefan on 04.04.2026.
//

#pragma once
#include <Jolt/Jolt.h>

#include "Jolt/Core/Reference.h"
#include "Jolt/Physics/Body/MotionType.h"
#include "Utils/Math/Vector.hpp"

namespace JPH {
    class Shape;
}

namespace Obj3D {
    struct PhysicsSettings {

        const JPH::RefConst<JPH::Shape> shape ;
        ggm::Vector3f pos;
        ggm::Vector3f rot;

        JPH::EMotionType motionType;
    };

    namespace PhysicsFactory {
        PhysicsSettings newBox(float x, float y, float z, const ggm::Vector3f &pos, JPH::EMotionType motionType = JPH::EMotionType::Dynamic);
    }
} // Obj