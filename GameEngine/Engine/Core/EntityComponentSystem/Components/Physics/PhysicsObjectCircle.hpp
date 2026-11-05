//
// Created by Stefan on 05.11.2026.
//

#pragma once
#include "Math/Matrix.hpp"
#include "Math/Vector.hpp"

namespace ggm {
    struct PhysicsObjectCircle {
        Vector2f position;
        float radius;

        Vector2f velocity;
        Vector2f acceleration;
        float mass;
    };
}
