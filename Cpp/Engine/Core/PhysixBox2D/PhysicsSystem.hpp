// Created by Stefan on 27.03.2026.
//

#pragma once
#include "../EngineDefines.hpp"
#include "Render/Objects/2D/Physics/PhysicsObjectRef.hpp"
#include "Utils/DataStorage/SparseSet.hpp"

namespace Game2D::PhysicsFactory2D {
    struct PhysicsSettings;
}

namespace PhysixBox {
    class PhysicsSystem {
        struct Collision {
            PointMass* a = nullptr;
            PointMass* b = nullptr;
            ggm::Vector2f point{};
            ggm::Vector2f normal{};
            float distance = 0;
        };

        ggm::SparseSet<SoftBody2D> mBodies{16};

        static void checkIntersection(SoftBody2D& a, SoftBody2D& b);

        static void resolveCollision(const Collision &c, PointMass &p);

        static bool isInsideV2(const SoftBody2D &a, const ggm::Vector2f &point);
        static bool isInside(SoftBody2D &a, const ggm::Vector2f &point);
        static Collision getCollision(SoftBody2D &body, const ggm::Vector2f& p);

    public:
        void update(int steps, float dt);

        template<typename... Args>
        ggm::u64 addBody(Args&&... args) {
            return mBodies.push(std::forward<Args>(args)...);
        }

        Game2D::PhysicsObjectRef addBody(const Game2D::PhysicsFactory2D::PhysicsSettings &settings);

    };
} // PhysicsBox