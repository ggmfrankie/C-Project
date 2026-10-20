//
// Created by Stefan on 18.10.2026.
//
#include "BattleScene.hpp"

#include "Engine/Core/EntityComponentSystem/Core/RenderContext.hpp"
#include "Engine/Core/EntityComponentSystem/Systems/Physics/PlayerController.hpp"
#include "Engine/Core/EntityComponentSystem/Systems/Render/SpriteLoader.hpp"
#include "Engine/Core/EntityComponentSystem/Systems/Render/SpriteRenderer.hpp"

namespace ggmTanks {
    void BattleScene::init() {
        mComponentRegistry.createEntity({}, ggm::SpriteLocation{"ERROR.png"}, ggm::Transformation{{100, 100}, {40, 20}});
        mComponentRegistry.createEntity({},
            ggm::SpriteLocation{"Black.png"},
            ggm::Transformation{{100, 200}, {80, 70}},
            ggm::PlayerControlled{}
        );

        ggm::SpriteLoader::load(mComponentRegistry);
    }

    void BattleScene::render(int width, int height) {
        ggm::SpriteRenderer::draw(mComponentRegistry, {width, height});
    }

    void BattleScene::update(float dt) {
        ggm::PlayerController::move(mComponentRegistry);
    }
}
