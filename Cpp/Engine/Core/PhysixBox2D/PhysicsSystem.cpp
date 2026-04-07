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
                Collision c = getCollision(a, p);
                point.pos += c.normal*c.distance;
                c.a->pos += -c.normal*c.distance;
                c.b->pos += -c.normal*c.distance;
                puts("collision");
            }
        }

        for (auto& point : a.getPoints()) {
            if (auto& p = point.pos; isInside(b, p)) {
                Collision c = getCollision(b, p);
                point.pos += c.normal*c.distance;
                c.a->pos += -c.normal*c.distance;
                c.b->pos += -c.normal*c.distance;
                puts("collision");
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

    PhysicsSystem::Collision PhysicsSystem::getCollision(SoftBody2D& body, const ggm::Vector2f& p) {
        using namespace ggm;
        using vec2 = Vector2f;

        Collision result{};
        float minDistance = std::numeric_limits<float>::max();

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

            if (const float distSq = vec2::distanceSquared(pointOnLine, p); distSq < minDistance) {
                minDistance = distSq;

                result.a = &pma;
                result.b = &pmb;
                result.normal = (pointOnLine - p).normalize();
                result.point = pointOnLine;
            }
        }
        result.distance = std::sqrt(minDistance);
        return result;
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

            body.addSpring(first, second, 200000.0f, length, 20.0f);
        }

        return {id, mBodies};
    }


} // PhysicsBox