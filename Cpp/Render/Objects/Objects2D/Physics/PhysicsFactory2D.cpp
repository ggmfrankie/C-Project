//
// Created by Stefan on 05.04.2026.
//

#include "PhysicsFactory2D.hpp"

#include "Utils/Math/Matrix.hpp"

namespace Obj2D::PhysicsFactory2D {
    using namespace ggm;
    PhysicsSettings newQuad(float x, float y, Vector2f pos, float rot) {
        PhysicsSettings ps;

        const float a = x/2;
        const float b = y/2;

        const Matrix2f r = Matrix2f::Rotation(rot);

        ps.points.emplace_back(pos + r*Vector2f{-a, -b});
        ps.points.emplace_back(pos + r*Vector2f{a, -b});
        ps.points.emplace_back(pos + r*Vector2f{-a, b});
        ps.points.emplace_back(pos + r*Vector2f{a, b});

        ps.connections.emplace_back(0, 1);
        ps.connections.emplace_back(0, 2);
        ps.connections.emplace_back(1, 2);
        ps.connections.emplace_back(1, 3);
        ps.connections.emplace_back(2, 3);
        ps.connections.emplace_back(3, 0);

        return ps;
    }
    PhysicsSettings newSoftBall(int segments, float radius, Vector2f pos) {
        PhysicsSettings ps;

        for (int i = 0; i < segments; i++) {
            float t = (float)i / segments;
            float angle = t * 2.0f * M_PI;

            Vector2f p = pos + Vector2f{
                std::cos(angle) * radius,
                std::sin(angle) * radius
            };

            ps.points.emplace_back(p);
        }

        // ring connections (this is what your strip needs)
        for (int i = 0; i < segments; i++) {
            ps.connections.emplace_back(i, (i + 1) % segments);
        }

        return ps;
    }
} // Obj