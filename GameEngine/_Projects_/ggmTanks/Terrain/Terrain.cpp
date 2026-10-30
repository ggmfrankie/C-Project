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

    MeshData2D Terrain::reloadMesh() {
        MeshData2D out {};

        out.vertices = mHeightmap;
        out.vertices.emplace_back(0, static_cast<float>(mDepth));
        const int leftCornerIndex = out.vertices.size()-1;

        out.vertices.emplace_back(mHeightmap.back().x, static_cast<float>(mDepth));
        const int rightCornerIndex = out.vertices.size()-1;


        const size_t lastInHeightmap = mHeightmap.size();
        for (int i = 1; i < lastInHeightmap; ++i) {
            out.indices.push_back(i);
            out.indices.push_back(i-1);
            out.indices.push_back(leftCornerIndex);
        }

        out.indices.push_back(lastInHeightmap);
        out.indices.push_back(lastInHeightmap-1);
        out.indices.push_back(rightCornerIndex);

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
        for (int i = 0; i < width; ++i) {
            mHeightmap.emplace_back(i, rand()%20 - 10);
        }
    }

    void Terrain::registerMesh(Registry& registry) {
        registry.createEntity(mMeshEntity,
            Transformation{{1, 1}, {}},
            reloadMesh(),
            TextureLocation{"Black.png"}
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