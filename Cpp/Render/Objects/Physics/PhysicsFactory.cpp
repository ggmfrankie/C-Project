//
// Created by Stefan on 04.04.2026.
//
#include "PhysicsFactory.hpp"

#include "Jolt/Physics/Collision/Shape/BoxShape.h"

namespace Obj3D::PhysicsFactory {
    using namespace JPH;
    using namespace ggm;
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
