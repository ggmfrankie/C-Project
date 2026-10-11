//
// Created by Stefan on 04.04.2026.
//
#include "PhysicsFactory3D.hpp"

#include "Jolt/Physics/Collision/Shape/BoxShape.h"

namespace ggm::PhysicsFactory {
    using namespace JPH;
    using namespace ggm;

    //! @brief Creates the settings for a Box with dimensions x,y,z at given pos
    PhysicsSettings newBox(float x, float y, float z, const Vector3f &pos, EMotionType motionType) {
        static std::unordered_map<Vector3f, RefConst<Shape>> duplicates{16};

        const Vector3f index{x,y,z};
        const auto it = duplicates.find(index);

        const RefConst<Shape> shape = (it != duplicates.end()) ? it->second: new BoxShape(Vec3(x/2,y/2,z/2));
        if (it == duplicates.end()) duplicates[index] = shape;

        return {
            .shape = shape,
            .pos = pos,
            .rot = {},
            .motionType = motionType,
        };
    }
}
