//
// Created by Stefan on 05.04.2026.
//

#pragma once
#include <vector>

#include "Utils/Math/ggmdef.hpp"
#include "Utils/Math/Vector.hpp"

namespace Obj2D::PhysicsFactory2D {
    struct PhysicsSettings {
        std::vector<ggm::Vector2f> points;
        std::vector<std::pair<ggm::u64, ggm::u64>> connections;
    };

    PhysicsSettings newQuad(float x, float y, ggm::Vector2f pos, float rot = 0);
    PhysicsSettings newSoftBall(int segments, float radius, ggm::Vector2f pos);
} // Obj