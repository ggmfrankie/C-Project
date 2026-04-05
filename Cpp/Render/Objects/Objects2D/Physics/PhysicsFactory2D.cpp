//
// Created by Stefan on 05.04.2026.
//

#include "PhysicsFactory2D.hpp"

namespace Obj2D::PhysicsFactory2D {
    using namespace ggm;
    PhysicsSettings newQuad(float x, float y, Vector2f pos) {
        PhysicsSettings ps;

        const float a = x/2;
        const float b = x/2;

        ps.points.emplace_back(pos + Vector2f{a, b});
        ps.points.emplace_back(pos + Vector2f{-a, b});
        ps.points.emplace_back(pos + Vector2f{a, -b});
        ps.points.emplace_back(pos + Vector2f{-a, -b});

        ps.connections.emplace_back(0, 1);
        ps.connections.emplace_back(1, 2);
        ps.connections.emplace_back(2, 3);
        ps.connections.emplace_back(3, 0);

        return ps;
    }
} // Obj