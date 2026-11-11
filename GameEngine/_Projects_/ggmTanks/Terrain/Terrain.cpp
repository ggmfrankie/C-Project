//
// Created by Stefan on 20.10.2026.
//

#include "Terrain.hpp"

#include <algorithm>
#include <assert.h>

#include "GuiInterface.h"
#include "Engine/Core/EntityComponentSystem/Components/Render/MeshData2D.hpp"
#include "Engine/Core/EntityComponentSystem/Systems/Loader/MeshLoader2D.hpp"
#include "GUIEngine/Utils/Math/Math.hpp"

namespace ggmTanks {
    using namespace ggm;
    using std::filesystem::path;

    std::vector<Vector2f> generateRandomHeightmap(int width);

    MeshData2D Terrain::reloadMesh() {
        MeshData2D out {};

        out.vertices = mHeightmap;
        out.vertices.emplace_back(0, static_cast<float>(mDepth));

        const size_t lastInHeightmap = mHeightmap.size();

        out.indices.push_back(1);
        out.indices.push_back(0);
        out.vertices.emplace_back(mHeightmap[0].x, static_cast<float>(mDepth));
        out.indices.push_back(out.vertices.size()-1);

        for (int i = 2; i < lastInHeightmap; ++i) {
            out.indices.push_back(i);
            out.indices.push_back(i-1);
            out.vertices.emplace_back(mHeightmap[i-1].x, static_cast<float>(mDepth));
            out.indices.push_back(out.vertices.size()-1);

            out.indices.push_back(i-1);
            out.indices.push_back(out.vertices.size()-2);
            out.indices.push_back(out.vertices.size()-1);
        }

        out.indices.push_back(lastInHeightmap-1);
        out.indices.push_back(out.vertices.size()-1);
        out.vertices.emplace_back(mHeightmap[lastInHeightmap-1].x, static_cast<float>(mDepth));
        out.indices.push_back(out.vertices.size()-1);

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
        mHeightmap(generateRandomHeightmap(width)),
        mDepth(depth),
        mMeshDirty(true)
    {}

    std::vector<Vector2f> generateRandomHeightmap(int width) {
        std::vector<Vector2f> out;

        int height = 0;
        for (int i = 0; i < width; i += 10) {
            const int offset = ((rand()%20 -10)?:1);
            height += rand() % offset - offset/2+1;
            out.emplace_back(i, height);
        }
        return out;
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
        if (mMeshDirty || gui_wasClicked("terrain_reload")) {
            mHeightmap = generateRandomHeightmap(1000);
            registry.changeComponents(mMeshEntity, TypeList<Mesh2D, SimpleCollider>{}, reloadMesh(), SimpleCollider{mHeightmap, {0,500}});
            MeshLoader2D::load(registry);
            mMeshDirty = false;
        }
    }
} // ggmTanks