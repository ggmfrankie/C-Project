//
// Created by ertls on 09.03.2026.
//

#include "TestBehavior.hpp"
#include "Render/IO/Input.hpp"

namespace Obj {

    void TestBehavior::onCollision(PhysicsObject_old& self, PhysicsObject_old& other) {
        std::cout << "Collision\n";
    }

    void TestBehavior::onUpdate(PhysicsObject_old &self, const double dt, Render::Input &input) {

    }
} // Obj