// Created by Stefan on 27.03.2026.
//

#pragma once
#include "Render/Objects/Objects2D/Physics/PhysicsObjectRef2D.hpp"
#include "Utils/DataStorage/SparseSet.hpp"

namespace Obj2D::PhysicsFactory2D {
    struct PhysicsSettings;
}

namespace PhysixBox {
    class PhysicsSystem {
        ggm::SparseSet<SoftBody2D> mBodies{16};

    public:
        void update(int steps, float dt);

        template<typename... Args>
        ggm::u64 addBody(Args&&... args) {
            return mBodies.push(std::forward<Args>(args)...);
        }

        Obj2D::PhysicsObjectRef2D addBody(const Obj2D::PhysicsFactory2D::PhysicsSettings &settings);

    };
} // PhysicsBox