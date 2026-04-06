//
// Created by Stefan on 06.04.2026.
//

#pragma once
#include "Utils/Math/Matrix.hpp"

namespace PhysixBox {
    struct Collision {
        ggm::Vector2f normal{};
        float depth = -std::numeric_limits<float>::infinity();
    };
} // PhysixBox