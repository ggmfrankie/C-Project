//
// Created by Stefan on 18.10.2026.
//
#include "BattleScene.hpp"

#include "Engine/Core/EntityComponentSystem/Core/RenderContext.hpp"
#include "Engine/Core/EntityComponentSystem/Systems/Render/SpriteLoader.hpp"
#include "Engine/Core/EntityComponentSystem/Systems/Render/SpriteRenderer.hpp"

namespace ggm {
    void BattleScene::init() {
        mComponentRegistry.createEntity({}, SpriteLocation{"ERROR.png"}, Transformation{{100, 100}, {40, 20}});
        mComponentRegistry.createEntity({}, SpriteLocation{"Black.png"}, Transformation{{100, 200}, {80, 70}});
        SpriteLoader::load(mComponentRegistry);
    }

    void BattleScene::render(int width, int height) {
        SpriteRenderer::draw(mComponentRegistry, {width, height});
    }

    void BattleScene::update(float dt) {

    }
}
