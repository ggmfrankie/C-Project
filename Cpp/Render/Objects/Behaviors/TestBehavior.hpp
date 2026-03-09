//
// Created by ertls on 09.03.2026.
//
#pragma once
#include "IBehavior.hpp"

namespace Obj {
    class TestBehavior: public IBehavior {
    public:
        TestBehavior() = default;

        void onCollision(PhysicsObject& self, PhysicsObject& other) override;
        void onUpdate(PhysicsObject& self, float dt, Render::Input& input) override;
    };
} // Obj

