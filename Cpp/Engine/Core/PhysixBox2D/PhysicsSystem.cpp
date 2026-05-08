//
// Created by Stefan on 27.03.2026.
//

#include "PhysicsSystem.hpp"

#include <cmath>

#include "Collision.hpp"
#include "Render/Objects/Objects2D/Physics/PhysicsFactory.hpp"

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

                for (auto& point : obj.getPoints()) {
                    if (constexpr float groundY = 0.0f; point.pos.y < groundY) {
                        // 1. Position correction (MANDATORY)
                        point.pos.y = groundY;

                        // 2. Only react if moving INTO the ground
                        if (point.vel.y < 0.0f) {
                            constexpr float restitution = 0.3f; // 0 = sticky, 1 = bouncy
                            point.vel.y = -point.vel.y * restitution;
                        }

                        // 3. Friction (tangential damping)
                        constexpr float friction = 0.8f;
                        point.vel.x *= friction;
                    }
                }
            }
        }
    }

    void PhysicsSystem::checkIntersection(SoftBody2D &a, SoftBody2D &b) {
        using vec2 = ggm::Vector2f;
        for (auto& point : b.getPoints()) {
            if (auto& p = point.pos; isInsideV2(a, p)) {
                Collision c = getCollision(a, p);
                resolveCollision(c, point);
            }
        }

        for (auto& point : a.getPoints()) {
            if (auto& p = point.pos; isInsideV2(b, p)) {
                Collision c = getCollision(b, p);
                resolveCollision(c, point);
            }
        }
    }

    void PhysicsSystem::resolveCollision(const Collision& c, PointMass& p) {

        using vec2 = ggm::Vector2f;

        const float invA = 1.0f / c.a->mass;
        const float invB = 1.0f / c.b->mass;
        const float invP = 1.0f / p.mass;

        const float invSum = invA + invB + invP;
        if (invSum == 0) return;

        const vec2 correction = c.normal * c.distance;

        c.a->pos -= correction * (invA / invSum);
        c.b->pos -= correction * (invB / invSum);
        p.pos    += correction * (invP / invSum);

        c.a->vel -= correction * (invA / invSum) *100;
        c.b->vel -= correction * (invB / invSum) *100;
        p.vel    += correction * (invP / invSum) *100;

    }

    bool PhysicsSystem::isInsideV2(const SoftBody2D& a, const ggm::Vector2f& point) {
        using namespace ggm;
        using vec2 = Vector2f;

        const auto& points = a.getPoints();
        const u64 length = points.size();
        bool inside = false;

        for (int i = 0; i < length; ++i) {
            const vec2 p0 = points[i].pos;
            const vec2 p1 = points[(i+1)%length].pos;

            if (const bool yCheck = (p0.y > point.y) != (p1.y > point.y);
                !yCheck) continue;
            if (const float xIntersect = (p1.x - p0.x) * (point.y - p0.y) / (p1.y - p0.y) + p0.x;
                point.x < xIntersect) inside = !inside;
        }
        return inside;
    }

    bool PhysicsSystem::isInside(SoftBody2D& a, const ggm::Vector2f& point) {
        using namespace ggm;
        using vec2 = Vector2f;

        auto& points = a.getPoints();
        const u64 length = points.size();
        int intersections = 0;

        for (int i = 0; i < length; ++i) {
            vec2 p0 = points[i].pos;
            vec2 p1 = points[(i+1)%length].pos;

            //is point behind the segment?
            if (point.x > p0.x && point.x > p1.x) continue;

            //is point below or above the segment?
            if (point.y > p0.y && point.y > p1.y) continue;
            if (point.y < p0.y && point.y < p1.y) continue;

            const float cross = vec2::cross(point-p0, p1-p0);

            //deduplicate
            constexpr float EPS = 1e-6f;
            if (p0.y >= p1.y) {
                if (std::abs(point.y - p0.y) < EPS || cross < 0) continue;
                intersections++;
            } else {
                if (std::abs(point.y - p1.y) < EPS || cross > 0) continue;
                intersections++;
            }
        }
        return intersections % 2;
    }

    PhysicsSystem::Collision PhysicsSystem::getCollision(SoftBody2D& body, const ggm::Vector2f& p) {
        using namespace ggm;
        using vec2 = Vector2f;

        Collision result{};
        float minDistSq = std::numeric_limits<float>::max();

        auto& points = body.getPoints();
        const u64 length = points.size();

        for (u64 i = 0; i < length; i++) {
            auto& pma = points[i];
            auto& pmb = points[(i+1)%length];

            vec2 a = pma.pos;
            vec2 b = pmb.pos;

            vec2 ab = b - a;
            vec2 ap = p - a;

            const float scalar = vec2::dot(ab, ap);

            const float abLenSq = ab.lengthSquared();
            const float d = scalar/abLenSq;

            vec2 pointOnLine;

            if (d <= 0) pointOnLine = a;
            else if (d >= 1) pointOnLine = b;
            else pointOnLine = a + ab * d;

            if (const float distSq = vec2::distanceSquared(pointOnLine, p); distSq < minDistSq) {
                minDistSq = distSq;

                result.a = &pma;
                result.b = &pmb;
                result.normal = (pointOnLine - p).normalize();
                result.point = pointOnLine;
            }
        }
        result.distance = std::sqrt(minDistSq);
        return result;
    }

    Game2D::PhysicsObjectRef PhysicsSystem::addBody(const Game2D::PhysicsFactory2D::PhysicsSettings& settings) {
        const auto id = mBodies.push();
        auto& body = mBodies.get(id);

        for (auto& point: settings.points) {
            body.addPoint(point);
        }

        for (const auto&[a, b] : settings.connections) {
            auto& first = body.getPointMass(a);
            auto& second = body.getPointMass(b);

            float length = (first.pos - second.pos).length();

            body.addSpring(first, second, 200.0f, length, 10.0f);
        }

        return {id, mBodies};
    }


} // PhysicsBox