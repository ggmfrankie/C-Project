//
// Created by Stefan on 20.10.2026.
//

#pragma once
#include <array>
#include <vector>

#include "ggmdef.hpp"
#include "Math/Vector.hpp"

namespace ggmTanks {
    class TerrainHandler {
        static constexpr int WIDTH  = 64;
        static constexpr int HEIGHT = 32;

        std::array<float, WIDTH*HEIGHT> mDensityField {};

        ggm::u32 mWidth  = 0;
        ggm::u32 mHeight = 0;

    public:
        using TerrainOutline = std::vector<ggm::Vector2f>;

        explicit TerrainHandler(const TerrainOutline& outline);
    };
} // ggmTanks
