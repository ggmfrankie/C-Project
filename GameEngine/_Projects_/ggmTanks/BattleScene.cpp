//
// Created by Stefan on 18.10.2026.
//
#include "BattleScene.hpp"

#include "Engine/Core/EntityComponentSystem/Core/RenderContext.hpp"
#include "Engine/Core/EntityComponentSystem/Systems/Physics/PlayerController.hpp"
#include "Engine/Core/EntityComponentSystem/Systems/Render/MeshRenderer2D.hpp"
#include "Engine/Core/EntityComponentSystem/Systems/Render/SpriteLoader.hpp"
#include "Engine/Core/EntityComponentSystem/Systems/Render/SpriteRenderer.hpp"

namespace ggmTanks {
    using namespace ggm;
    void BattleScene::init() {
        mComponentRegistry.createEntity({}, SpriteLocation{"ERROR.png"}, Transformation{{100, 100}, {40, 20}});
        mComponentRegistry.createEntity({},
            SpriteLocation{"Black.png"},
            Transformation{{100, 200}, {80, 70}},
            PlayerControlled{}
        );

        SpriteLoader::load(mComponentRegistry);
    }

    void BattleScene::render(int width, int height) {
        const RenderContext context {width, height};

        MeshRenderer2D::draw(mComponentRegistry, context);
        SpriteRenderer::draw(mComponentRegistry, context);
    }

    void BattleScene::update(float dt) {
        PlayerController::move(mComponentRegistry);
    }
}
