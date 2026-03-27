//
// Created by Stefan on 27.03.2026.
//

#include "PhysicsSystem.hpp"

namespace PhysixBox {
    void PhysicsSystem::update(int steps, float dt) {
        for (int i = 0; i < steps; i++) {
            for (auto& obj: mBodies) {
                obj.update(dt);
            }
        }
    }
} // PhysicsBox