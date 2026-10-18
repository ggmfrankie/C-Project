//
// Created by Stefan on 18.10.2026.
//
#include "BattleScene.hpp"

#include "Engine/Core/EntityComponentSystem/Systems/Render/SpriteLoader.hpp"
#include "Engine/Core/EntityComponentSystem/Systems/Render/SpriteRenderer.hpp"

namespace ggm {
    void BattleScene::init() {
        mComponentRegistry.createEntity({}, SpriteLocation{"ERROR.png"}, Position{100, 100});
        SpriteLoader::load(mComponentRegistry);
    }

    void BattleScene::render(int width, int height) {
        SpriteRenderer::draw(mComponentRegistry);
    }

    void BattleScene::update(float dt) {

    }

    Camera& BattleScene::getCamera() {

    }
}
