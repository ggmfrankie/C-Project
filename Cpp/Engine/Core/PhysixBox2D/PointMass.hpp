//
// Created by Stefan on 27.03.2026.
//

#pragma once
#include "Constrains/Spring.hpp"
#include "Utils/Math/Vector.hpp"


namespace PhysixBox {
    struct PointMass {
        ggm::Vector2f mPos{};
        ggm::Vector2f mVelocity{};
        ggm::Vector2f mForce{};

        float mMass = 1.0;

        void integrate(float dt);
        void addForce(const ggm::Vector2f& force);
    };
} // PhysixBox