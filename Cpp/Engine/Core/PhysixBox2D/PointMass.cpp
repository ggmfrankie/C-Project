//
// Created by Stefan on 27.03.2026.
//

#include "PointMass.hpp"

namespace PhysixBox {
    using namespace ggm;

    void PointMass::integrate(float dt) {
        mVelocity += (mForce * mMass) * dt;
        mPos += mVelocity * dt;

        mForce = {};
    }

    void PointMass::addForce(const Vector2f& force) {
        mForce += force;
    }
} // PhysixBox