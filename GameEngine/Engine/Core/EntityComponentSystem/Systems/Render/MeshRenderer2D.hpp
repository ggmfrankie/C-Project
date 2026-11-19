//
// Created by Stefan on 23.10.2026.
//

#pragma once
#include "../../ComponentRegistry.hpp"
#include "../../Components/Render/Mesh2D.hpp"
#include "../../Components/Transformation.hpp"
#include "../../Core/RenderContext.hpp"
#include "../../Components/Render/Texture.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Render/Color.hpp"

namespace ggm::MeshRenderer2D {

    void drawMeshes(
        std::span<const Mesh2D> meshes,
        std::span<const Texture> textures,
        std::span<const Color> colors,
        std::span<const Transformation> transformations
    );

    void drawMeshes(
        std::span<const Mesh2D> meshes,
        std::span<const Texture> textures,
        std::span<const Transformation> transformations
    );

    void drawMeshes(
        std::span<const Mesh2D> meshes,
        std::span<const Color> colors,
        std::span<const Transformation> transformations
    );

    void drawBegin(const RenderContext& context);

    template<typename... Components>
    requires ContainsType<Mesh2D, Components...> && ContainsType<Transformation, Components...> && ContainsType<Texture, Components...> && ContainsType<Color, Components...>
    void draw(ComponentRegistry<Components...>& registry, const RenderContext& context) {

        drawBegin(context);

        for (auto&[count, entities, meshes, textures, colors, transformations] : registry.getEntities(All<Mesh2D, Texture, Color, Transformation>{})) {
            drawMeshes(meshes, textures, colors, transformations);
        }

        for (auto&[count, entities, meshes, textures, transformations] : registry.getEntities(All<Mesh2D, Texture, Transformation>{}, Not<Color>{})) {
            drawMeshes(meshes, textures, transformations);
        }

        for (auto&[count, entities, meshes, colors, transformations] : registry.getEntities(All<Mesh2D, Color, Transformation>{}, Not<Texture>{})) {
            drawMeshes(meshes, colors, transformations);
        }
    }
}
