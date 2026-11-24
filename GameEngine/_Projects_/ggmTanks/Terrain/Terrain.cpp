//
// Created by Stefan on 20.10.2026.
//

#include "Terrain.hpp"

#include <algorithm>
#include <assert.h>

#include "GuiInterface.h"
#include "Engine/Core/EntityComponentSystem/Components/Render/MeshData2D.hpp"
#include "Engine/Core/EntityComponentSystem/Systems/IO/InputHandler.hpp"
#include "Engine/Core/EntityComponentSystem/Systems/Loader/MeshLoader2D.hpp"
#include "_Projects_/ggmTanks/Components/Impact.hpp"

namespace ggmTanks {
    using namespace ggm;

    std::vector<Vector2f> generateRandomHeightmap(int width);

    MeshData2D Terrain::reloadMesh() {
        MeshData2D out {};

        out.vertices = mHeightmap;
        out.vertices.emplace_back(0, mDepth);

        const size_t lastInHeightmap = mHeightmap.size();

        out.indices.push_back(1);
        out.indices.push_back(0);
        out.vertices.emplace_back(mHeightmap[0].x, mDepth);
        out.indices.push_back(out.vertices.size()-1);

        for (int i = 2; i < lastInHeightmap; ++i) {
            out.indices.push_back(i);
            out.indices.push_back(i-1);
            out.vertices.emplace_back(mHeightmap[i-1].x, mDepth);
            out.indices.push_back(out.vertices.size()-1);

            out.indices.push_back(i-1);
            out.indices.push_back(out.vertices.size()-2);
            out.indices.push_back(out.vertices.size()-1);
        }

        out.indices.push_back(lastInHeightmap-1);
        out.indices.push_back(out.vertices.size()-1);
        out.vertices.emplace_back(mHeightmap[lastInHeightmap-1].x, mDepth);
        out.indices.push_back(out.vertices.size()-1);

        const float maxWidth = mHeightmap.back().x;

        for (auto [x, y] : out.vertices) {
            out.uvs.emplace_back(x/maxWidth, y/mDepth);
        }

        return out;
    }

    Terrain::Terrain(int width, int depth) :
        mHeightmap(generateRandomHeightmap(width)),
        mDepth(depth),
        mWidth(width),
        mPos(0, 500),
        mMeshDirty(true)
    {}

    std::vector<Vector2f> generateRandomHeightmap(int width) {
        std::vector<Vector2f> out;
        int height = 0;

        for (int i = 0; i < width; i += 10) {
            const int offset = ((rand()%20 -10)?:1);
            height += offset;
            out.emplace_back(i, height);
        }
        return out;
    }

    void Terrain::registerMesh(Registry& registry) {
        registry.createEntity(mMeshEntity,
            Transformation{{1, 1}, mPos},
            reloadMesh(),
            TextureLocation{"ERROR.png"},
            Color::BLUE,
            SimpleCollider{mHeightmap, mPos}
        );
    }

    void Terrain::handleImpacts(Registry& registry) {
        auto handleImpact = [this](const Impact& impact) {
            for (auto& [x, y] : mHeightmap) {
                const float worldX = x + mPos.x;
                const float worldY = y + mPos.y;

                const float dx = worldX - impact.pos.x;

                if (std::abs(dx) >= impact.radius) continue;

                // Pythagoras
                const float dy = std::sqrtf(
                    impact.radius * impact.radius - dx * dx
                );

                float craterBottom = impact.pos.y + dy;

                y = std::max(worldY, craterBottom) - mPos.y;
            }
        };

        for (auto& [count, entities, impacts] : registry.popEntities(All<Impact>{})) {
            for (auto& impact : impacts) {
                Logger::debug({"Handling Impact at pos {} with radius {}", impact.pos, impact.radius});
                handleImpact(impact);
            }
            mMeshDirty = true;
        }
    }

    void Terrain::update(Registry& registry) {
        Logger::Suppress _ {};
        if (InputHandler::isMouseButtonPressed(MouseButton::LEFT)) {
            spawnImpact(registry, InputHandler::getCursorPos());
        }

        handleImpacts(registry);

        if (gui_wasClicked("terrain_reload")) {
            mHeightmap = generateRandomHeightmap(mWidth);
            mMeshDirty = true;
        }

        if (mMeshDirty) {
            Logger::debug({"Reloading mesh"});
            registry.changeComponents(
                mMeshEntity,
                Not<Mesh2D, SimpleCollider>{},
                reloadMesh(),
                SimpleCollider{mHeightmap, mPos}
            );
            
            MeshLoader2D::load(registry);
            mMeshDirty = false;
        }
    }

    void Terrain::spawnImpact(Registry& registry, Vector2f pos) {
        registry.createEntity({}, Impact{.pos = pos, .radius = static_cast<float>(rand()%100 +50)});
    }
} // ggmTanks