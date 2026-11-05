//
// Created by Stefan on 20.10.2026.
//

#include "Terrain.hpp"

#include <algorithm>
#include <assert.h>

#include "Engine/Core/EntityComponentSystem/Components/Render/MeshData2D.hpp"
#include "Engine/Core/EntityComponentSystem/Systems/Loader/MeshLoader2D.hpp"
#include "GUIEngine/Utils/Math/Math.hpp"

namespace ggmTanks {
    using namespace ggm;
    using std::filesystem::path;

    MeshData2D Terrain::reloadMesh() {
        MeshData2D out {};

        out.vertices = mHeightmap;
        out.vertices.emplace_back(0, static_cast<float>(mDepth));

        const size_t lastInHeightmap = mHeightmap.size();
        for (int i = 1; i < lastInHeightmap; ++i) {
            out.indices.push_back(i);
            out.indices.push_back(i-1);
            out.vertices.emplace_back(mHeightmap[i-1].x, static_cast<float>(mDepth));
            out.indices.push_back(out.vertices.size()-1);
        }

        const float maxWidth = mHeightmap.back().x;

        for (auto [x, y] : out.vertices) {
            out.uvs.emplace_back(x/maxWidth, y/mDepth);
        }

        return out;
    }

    Terrain::Terrain(std::vector<Vector2f>&& heightmap) :
        mHeightmap(std::move(heightmap)),
        mMeshDirty(true)
    {}

    Terrain::Terrain(int width, int depth) :
        mDepth(depth),
        mMeshDirty(true)
    {
        int height = 0;
        for (int i = 0; i < width; ++i) {
            if (i % 5 == 0) height += rand()%10 - 5;
            mHeightmap.emplace_back(i, height);
        }
    }

    void Terrain::registerMesh(Registry& registry) {
        constexpr Vector2f pos {0, 500};

        registry.createEntity(mMeshEntity,
            Transformation{{1, 1}, pos},
            reloadMesh(),
            TextureLocation{"ERROR.png"},
            SimpleCollider{mHeightmap, pos}
        );
    }

    void Terrain::update(Registry& registry) {
        if (mMeshDirty) {
            registry.changeComponents(mMeshEntity, TypeList<Mesh2D>{}, reloadMesh());
            MeshLoader2D::load(registry);
            mMeshDirty = false;
        }
    }
} // ggmTanks