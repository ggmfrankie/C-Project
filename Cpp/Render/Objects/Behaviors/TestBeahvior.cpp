//
// Created by ertls on 09.03.2026.
//

#include "TestBehavior.hpp"
#include "Render/IO/Input.hpp"
#include "Render/Objects/Physics/PhysicsObject_old.hpp"

namespace Obj {

    void TestBehavior::onCollision(PhysicsObject_old& self, PhysicsObject_old& other) {
        std::cout << "Collision\n";
    }

    void TestBehavior::onUpdate(PhysicsObject_old &self, const double dt, Render::Input &input) {
        if (input.isKeyDown(GLFW_KEY_LEFT))
            self.applyForce({static_cast<float>(-100.0 * dt), 0, 0});
        if (input.isKeyDown(GLFW_KEY_RIGHT))
            self.applyForce({static_cast<float>(100.0 * dt), 0, 0});
        if (input.isKeyDown(GLFW_KEY_UP))
            self.moveBy(0, 1 * dt, 0);
        if (input.isKeyDown(GLFW_KEY_DOWN))
            self.moveBy(0, -1 * dt, 0);

        self.rotateBy(.3,.2,.1);
    }
} // Obj