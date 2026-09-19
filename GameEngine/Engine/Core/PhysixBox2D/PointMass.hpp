//
// Created by Stefan on 27.03.2026.
//

#pragma once
#include "../EngineDefines.hpp"
#include "Utils/Math/Vector.hpp"


namespace PhysixBox {
    struct PointMass {
        ggm::Vector2f pos{};
        ggm::Vector2f vel{};
        ggm::Vector2f force{};

        float mass = 100.0;

        explicit PointMass(const ggm::Vector2f &pos) : pos(pos) {}
    };
} // PhysixBox