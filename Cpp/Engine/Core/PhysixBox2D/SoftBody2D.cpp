//
// Created by Stefan on 27.03.2026.
//

#include "SoftBody2D.hpp"
#include <algorithm>
#include <execution>


namespace PhysixBox {
    using namespace ggm;

    SoftBody2D::SoftBody2D(const std::vector<PointMass>& points, const std::vector<Spring>& springs) {
        for (auto& point : points) {
            mPoints.add(point);
        }

        for (auto& spring : springs) {
            mSprings.add(spring);
        }
    }

    SoftBody2D::SoftBody2D(const std::vector<PointMass>& points) {
        for (auto& point : points) {
            mPoints.add(point);
        }
    }

    void SoftBody2D::update(float dt) {
        using vec2 = Vector2f;

        for (auto& point: mPoints) {
            point.vel += vec2{0, -90.81f} * dt;
        }

        for (auto& spring: mSprings) {
            PointMass& first = spring.first;
            PointMass& second = spring.second;

            const vec2& p0 = first.pos;
            const vec2& p1 = second.pos;

            const vec2& v0 = first.vel;
            const vec2& v1 = second.vel;

            vec2 delta = p1 - p0;
            constexpr float eps = 1e-6f;
            const float distance = std::max(delta.length(), eps);

            vec2 requiredDelta = delta * (spring.length / distance);
            vec2 force = spring.strength * (requiredDelta - delta);

            first.vel -= (force / first.mass) * dt;
            second.vel += (force / first.mass) * dt;

            vec2 dir = delta.normalize();

            const float vRel = vec2::dot(v1 - v0, dir);
            const float damping = std::exp(-spring.damping * dt);
            const float newVRel = vRel * damping;
            const float vRelDelta = newVRel -vRel;

            first.vel -= dir * (vRelDelta/2.0f);
            second.vel += dir * (vRelDelta/2.0f);
        }

        for (auto& point: mPoints) {
            point.pos += point.vel * dt;
            static int iterations = 0;
            //iterations++;
            if (iterations >= 100) {
                std::cout << "Position is: " << point.pos << '\n';
                iterations = 0;
            }

        }
    }

    PointMass& SoftBody2D::getPointMass(u64 index) {
        return mPoints[index];
    }

    std::vector<Vector2f> SoftBody2D::getPositions() {
        std::vector<Vector2f> points{};
        points.reserve(16);

        for (auto& pm : mPoints) {
            points.push_back(pm.pos);
        }
        return points;
    }

    InlineVector<PointMass, 16>& SoftBody2D::getPoints() {
        return mPoints;
    }
} // PhysicsBox