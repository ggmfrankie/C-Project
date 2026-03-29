//
// Created by Stefan on 27.03.2026.
//

#pragma once
#include "PointMass.hpp"
#include "Utils/DataStorage/InlineVector.hpp"
#include <vector>

namespace PhysixBox {
    class SoftBody2D {
        ggm::InlineVector<PointMass, 16> mPoints{};


    public:
        void update(float dt);

        std::vector<ggm::Vector2f> getPositions();
    };
} // PhysicsBox