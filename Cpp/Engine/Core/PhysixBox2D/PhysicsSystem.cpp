//
// Created by Stefan on 27.03.2026.
//

#include "PhysicsSystem.hpp"

#include <cmath>

#include "Render/Objects/Objects2D/Physics/PhysicsFactory2D.hpp"

namespace PhysixBox {
    void PhysicsSystem::update(int steps, float dt) {
        for (int ii = 0; ii < steps; ii++) {

            for (int i = 0; i < mBodies.size(); i++) {
                SoftBody2D& a = mBodies[i];
                for (int j = i+1; j < mBodies.size(); j++) {
                    SoftBody2D& b = mBodies[j];
                    checkIntersection(a, b);
                }
            }
            for (auto& obj: mBodies) {
                obj.update(dt/static_cast<float>(steps));

                constexpr float groundY = 0.0f;
                for (auto& point : obj.getPoints()) {
                    if (point.pos.y < groundY) {
                        point.vel = - point.vel;
                    }
                }
            }
        }
    }

    void PhysicsSystem::checkIntersection(SoftBody2D &a, SoftBody2D &b) {
        for (auto& point : b.getPoints()) {
            if (auto& p = point.pos; isInside(a, p)) {
                //puts("collision");
            }
        }
    }

    bool PhysicsSystem::isInside(SoftBody2D& a, const ggm::Vector2f& point) {
        using vec2 = ggm::Vector2f;
        using namespace ggm;

        auto& points = a.getPoints();
        const auto length = points.size();
        int intersections = 0;

        for (int i = 0; i < length; i++) {
            vec2 p0 = points[i].pos;
            vec2 p1 = points[(i+1)%length].pos;

            //is point behind the segment?
            if (point.x > p0.x && point.x > p1.x) continue;

            //is point below or above the segment?
            if (point.y > p0.y && point.y > p1.y) continue;
            if (point.y < p0.y && point.y < p1.y) continue;

            const float cross = Vector2f::cross(point-p0, p1-p0);

            //deduplicate
            constexpr float EPS = 1e-6f;
            if (p0.y >= p1.y) {
                if (std::fabs(point.y - p0.y) < EPS || cross < 0) continue;
                intersections++;
            } else {
                if (std::fabs(point.y - p1.y) < EPS || cross > 0) continue;
                intersections++;
            }
        }
        return intersections % 2;
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

            float length = ggm::Vector2f{first.pos - second.pos}.length();

            body.addSpring(first, second, 100.0f, length, 1.0f);
        }

        return {id, mBodies};
    }


} // PhysicsBox