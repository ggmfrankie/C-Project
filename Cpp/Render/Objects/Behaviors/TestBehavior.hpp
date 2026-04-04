//
// Created by ertls on 09.03.2026.
//
#pragma once
#include "IBehavior.hpp"

namespace Obj3D {
    class TestBehavior: public IBehavior {
    public:
        TestBehavior() = default;

        void onCollision(PhysicsObject_old& self, PhysicsObject_old& other) override;
        void onUpdate(PhysicsObject_old& self, double dt, Render::Input& input) override;
    };
} // Obj

