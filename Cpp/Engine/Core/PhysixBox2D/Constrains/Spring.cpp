//
// Created by Stefan on 27.03.2026.
//

#include "Spring.hpp"

#include "Engine/Core/PhysixBox2D/PointMass.hpp"
#include "Utils/Math/Vector.hpp"

namespace PhysixBox {
    using namespace ggm;
    Spring::Spring(PointMass& first, PointMass& second, float strength, float length) :
        first(first),
        second(second),
        mStrength(strength),
        mLength(length)
    {}

    void Spring::update() {
        using vec2 = Vector2f;

        const vec2 delta = second.mPos - first.mPos;
        const vec2 dir = delta.normalize();

        const float diff = delta.length() - mLength;
        vec2 force = (diff * -mStrength) * dir;

        vec2 relVel = second.mVelocity - first.mVelocity;
        float dampening = Vector2f::dot(dir, relVel);

        const vec2 forceD = (-mStrength * dampening) * dir;

        force += forceD;

        first.addForce(force);
        second.addForce(-force);
    }
} // PhysixBox