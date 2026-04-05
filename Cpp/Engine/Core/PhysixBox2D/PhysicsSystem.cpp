//
// Created by Stefan on 27.03.2026.
//

#include "PhysicsSystem.hpp"

#include "Render/Objects/Objects2D/Physics/PhysicsFactory2D.hpp"

namespace PhysixBox {
    void PhysicsSystem::update(int steps, float dt) {
        for (int i = 0; i < steps; i++) {
            for (auto& obj: mBodies) {
                obj.update(dt);
            }
        }
    }

    Obj2D::PhysicsObjectRef2D PhysicsSystem::addBody(const Obj2D::PhysicsFactory2D::PhysicsSettings &settings) {
        const auto id = mBodies.push();
        auto& body = mBodies.get(id);

        for (auto& point: settings.points) {
            body.addPoint(point);
        }

        for (const auto&[a, b] : settings.connections) {
            auto& first = body.getPointMass(a);
            auto& second = body.getPointMass(b);

            float length = ggm::Vector2f{first.mPos - second.mPos}.length();

            body.addSpring(first, second, 10.0f, length);
        }

        return {id, mBodies};
    }


} // PhysicsBox