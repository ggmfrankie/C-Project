//
// Created by Stefan on 20.10.2026.
//

#include "Terrain.hpp"

#include <algorithm>
#include <assert.h>

#include "GUIEngine/Utils/Math/Math.hpp"

namespace ggmTanks {
    using namespace ggm;

    Mesh2D Terrain::reloadMesh() {
        // TODO: reload mesh from density field
        Logger::error({"NYI"});
    }

    Terrain::Terrain(const std::span<const Vector2f>& outline) :
        mMeshDirty(true)
    {
        using vec2 = Vector2f;
        const size_t size = mDensityField.size();
        for (int i = 0; i < size; ++i) {
            vec2 point{static_cast<float>(i % size), static_cast<float>(i / size)};

            const float sign = Math::isInsidePolygon(outline, point) ? 1 : -1;

            mDensityField[i] = sign * Math::closestDistanceToPolygon(outline, point);
        }
    }

    void Terrain::update(Registry& registry) {
        if (mMeshDirty) {
            registry.changeComponents(mMeshEntity, TypeList<Mesh2D>{}, reloadMesh());
            mMeshDirty = false;
        }
    }
} // ggmTanks