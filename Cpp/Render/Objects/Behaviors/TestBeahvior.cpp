//
// Created by ertls on 09.03.2026.
//

#include "TestBehavior.hpp"
#include "Render/IO/Input.hpp"
#include "Render/Objects/Physics/PhysicsObject.hpp"

namespace Obj {

    void TestBehavior::onCollision(PhysicsObject& self, PhysicsObject& other) {
        std::cout << "Collision detected\n";
    }

    void TestBehavior::onUpdate(PhysicsObject &self, const double dt, Render::Input &input) {
        if (input.isKeyDown(GLFW_KEY_LEFT))
            self.moveBy(-1 * dt, 0, 0);
        if (input.isKeyDown(GLFW_KEY_RIGHT))
            self.moveBy(1 * dt, 0, 0);
    }
} // Obj