//
// Created by Stefan on 23.10.2026.
//

#pragma once
#include "Engine/Core/EntityComponentSystem/ComponentRegistry.hpp"
#include "../../Components/Render/Mesh2D.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Transformation.hpp"
#include "Engine/Core/EntityComponentSystem/Core/RenderContext.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Render/Texture.hpp"

namespace ggm::MeshRenderer2D {

    void drawMeshes(const RenderContext& context, const std::vector<Mesh2D>& meshes, const std::vector<Texture>& textures, const std::vector<Transformation>& transformations);

    template<typename... Components>
    requires ContainsType<Mesh2D, Components...> && ContainsType<Transformation, Components...> && ContainsType<Texture, Components...>
    void draw(ComponentRegistry<Components...>& registry, const RenderContext& context) {
        for (auto&[count, entities, meshes, textures, transformations] : registry.template getArchetypes<Mesh2D, Texture, Transformation>()) {
            drawMeshes(context, meshes, textures, transformations);
        }
    }
}
