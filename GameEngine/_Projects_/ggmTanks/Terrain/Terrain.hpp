//
// Created by Stefan on 20.10.2026.
//

#pragma once
#include <array>
#include <vector>

#include "ggmdef.hpp"
#include "Math/Vector.hpp"
#include "_Projects_/ggmTanks/Registry.hpp"

namespace ggmTanks {
    class Terrain {
        using TerrainOutline = std::vector<ggm::Vector2f>;

        static constexpr int WIDTH  = 64;
        static constexpr int HEIGHT = 32;

        std::array<float, WIDTH*HEIGHT> mDensityField {};

        ggm::u32 mWidth  = 0;
        ggm::u32 mHeight = 0;

        ggm::Entity mMeshEntity;
        bool mMeshDirty;

        ggm::Mesh2D reloadMesh();

    public:
        explicit Terrain(const std::span<const ggm::Vector2f>& outline);

        void update(Registry& registry);
    };
} // ggmTanks
