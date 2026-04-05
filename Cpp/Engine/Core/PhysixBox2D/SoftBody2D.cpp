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
        for (auto& spring: mSprings) {
            spring.update();
        }

        for (auto& point: mPoints) {
            point.force += {0,-1};
            point.mVelocity += (point.force * point.mMass) * dt;
            point.mPos += point.mVelocity * dt;
            point.force = {};
        }
    }

    PointMass& SoftBody2D::getPointMass(u64 index) {
        return mPoints[index];
    }

    std::vector<Vector2f> SoftBody2D::getPositions() {
        std::vector<Vector2f> verts;
        verts.reserve(16);

        for (auto& point: mPoints) {
            verts.push_back(point.mPos);
        }
        return verts;
    }
} // PhysicsBox