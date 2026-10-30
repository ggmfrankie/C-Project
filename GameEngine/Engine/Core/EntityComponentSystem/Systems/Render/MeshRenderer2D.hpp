//
// Created by Stefan on 23.10.2026.
//

#pragma once
#include "../../ComponentRegistry.hpp"
#include "../../Components/Render/Mesh2D.hpp"
#include "../../Components/Transformation.hpp"
#include "../../Core/RenderContext.hpp"
#include "../../Components/Render/Texture.hpp"

namespace ggm::MeshRenderer2D {

    void drawMeshes(
        const RenderContext& context,
        std::span<const Mesh2D> meshes,
        std::span<const Texture> textures,
        std::span<const Transformation> transformations
    );

    template<typename... Components>
    requires ContainsType<Mesh2D, Components...> && ContainsType<Transformation, Components...> && ContainsType<Texture, Components...>
    void draw(ComponentRegistry<Components...>& registry, const RenderContext& context) {
        for (auto&[count, entities, meshes, textures, transformations] : registry.template getArchetypes<Mesh2D, Texture, Transformation>()) {
            drawMeshes(context, meshes, textures, transformations);
        }
    }
}
