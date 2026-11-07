//
// Created by Stefan on 07.11.2026.
//

#pragma once
#include "Math/Vector.hpp"

namespace ggm {
    struct Color : Vector4f {
        constexpr Color() = default;

        constexpr Color(const Vector4f& other) :
            Vector4f(other)
        {}

        constexpr Color& operator=(const Vector4f& other) {
            return *this = Color{other};
        }

        constexpr Color(float r, float g, float b, float alpha = 1.f) :
            Vector4f(r, g, b, alpha)
        {}

        static const Color WHITE;
        static const Color BLACK;
        static const Color RED;
        static const Color GREEN;
        static const Color BLUE;
    };
    constexpr Color Color::WHITE{1.f, 1.f, 1.f, 1.f};
    constexpr Color Color::BLACK{0.f, 0.f, 0.f, 1.f};
    constexpr Color Color::RED{1.f, 0.f, 0.f, 1.f};
    constexpr Color Color::GREEN{0.f, 1.f, 0.f, 1.f};
    constexpr Color Color::BLUE{0.f, 0.f, 1.f, 1.f};
}
