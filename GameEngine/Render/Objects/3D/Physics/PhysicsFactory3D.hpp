//
// Created by Stefan on 04.04.2026.
//

#pragma once
#include "../EngineDefines.hpp"
#include <Jolt/Jolt.h>

#include "Jolt/Core/Reference.h"
#include "Jolt/Physics/Body/MotionType.h"
#include "Utils/Math/Vector.hpp"

namespace JPH {
    class Shape;
}

namespace ggm {
    struct PhysicsSettings {

        const JPH::RefConst<JPH::Shape> shape ;
        Vector3f pos;
        Vector3f rot;

        JPH::EMotionType motionType;
    };

    namespace PhysicsFactory {
        PhysicsSettings newBox(float x, float y, float z, const Vector3f &pos, JPH::EMotionType motionType = JPH::EMotionType::Dynamic);
    }
} // Obj