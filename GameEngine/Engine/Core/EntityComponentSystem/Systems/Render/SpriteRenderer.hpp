//
// Created by Stefan on 12.10.2026.
//

#pragma once
#include "Engine/Core/EntityComponentSystem/ComponentRegistry.hpp"
#include "../../Components/Render/Sprite.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Transformation.hpp"


namespace ggm {
    struct RenderContext;
}

namespace ggm::SpriteRenderer {
    void drawSprites(const RenderContext& context, const std::vector<Sprite>& sprites, const std::vector<Transformation>& transformations);

    template<typename... Components>
    requires ContainsType<Sprite, Components...> && ContainsType<Transformation, Components...>
    void draw(ComponentRegistry<Components...>& registry, const RenderContext& context) {
        for (auto&[count, entities, sprites, transformations] : registry.template getArchetypes<Sprite, Transformation>()) {
            drawSprites(context, sprites, transformations);
        }
    }
}
