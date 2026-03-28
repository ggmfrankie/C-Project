//
// Created by Stefan on 27.03.2026.
//

#pragma once
#include "PointMass.hpp"
#include "Utils/DataStorage/InlineVector.hpp"

namespace PhysixBox {
    class SoftBody2D {
        ggm::InlineVector<PointMass, 16> mPoints{};


    public:
        void update(float dt);
    };
} // PhysicsBox