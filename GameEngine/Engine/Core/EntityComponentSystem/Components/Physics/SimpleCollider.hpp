//
// Created by Stefan on 05.11.2026.
//

#pragma once
#include <vector>

#include "Math/Vector.hpp"

namespace ggm {
    struct SimpleCollider {
        std::vector<Vector2f> spline;
        Vector2f pos;
    };
}
