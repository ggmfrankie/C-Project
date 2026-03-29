//
// Created by Stefan on 27.03.2026.
//

#include "SoftBody2D.hpp"
#include <algorithm>
#include <execution>

namespace PhysixBox {
    using namespace ggm;

    void SoftBody2D::update(float dt) {
        std::for_each(std::execution::par_unseq, mPoints.begin(), mPoints.end(),
            [dt](PointMass& p) {
                p.integrate(dt);
            }
        );
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