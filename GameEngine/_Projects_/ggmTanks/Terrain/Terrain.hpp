//
// Created by Stefan on 20.10.2026.
//

#pragma once
#include <array>
#include <vector>

#include "ggmdef.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Render/MeshData2D.hpp"
#include "Math/Vector.hpp"
#include "_Projects_/ggmTanks/Registry.hpp"

namespace ggmTanks {
    class Terrain {
        static constexpr int WIDTH  = 64;
        static constexpr int HEIGHT = 32;

        std::vector<ggm::Vector2f> mHeightmap;

        ggm::u32 mDepth  = 0;

        ggm::Entity mMeshEntity {};
        bool mMeshDirty;

        ggm::MeshData2D reloadMesh();

    public:
        explicit Terrain(std::vector<ggm::Vector2f>&& heightmap);
        Terrain(int width, int depth);

        void registerMesh(Registry& registry);
        void update(Registry& registry);
    };
} // ggmTanks
