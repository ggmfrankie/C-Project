//
// Created by Stefan on 18.10.2026.
//
#include "BattleScene.hpp"

#include "GuiInterface.h"
#include "Engine/Core/EntityComponentSystem/Core/RenderContext.hpp"
#include "Engine/Core/EntityComponentSystem/Systems/Physics/PlayerController.hpp"
#include "Engine/Core/EntityComponentSystem/Systems/Render/MeshRenderer2D.hpp"
#include "Engine/Core/EntityComponentSystem/Systems/Loader/MeshLoader2D.hpp"
#include "Engine/Core/EntityComponentSystem/Systems/Loader/TextureLoader.hpp"
#include "Engine/Core/EntityComponentSystem/Systems/Physics/Physics2D.hpp"
#include "Engine/Core/EntityComponentSystem/Systems/Render/DebugRenderer.hpp"

namespace ggmTanks {
    using namespace ggm;
    BattleScene::BattleScene() :
        mTerrain(1000, 800)
    {}

    void BattleScene::init() {
        mComponentRegistry.createEntity({},
            MeshDataQuad{},
            TextureLocation{"ERROR.png"},
            Transformation{{100, 100}, {40, 20}}
        );

        mComponentRegistry.createEntity({},
            MeshDataQuad{},
            TextureLocation{"Black.png"},
            Transformation{{100, 200}, {80, 70}},
            PlayerControlled{},
            PhysicsObjectCircle{.radius = 200, .mass = 1}
        );

        mTerrain.registerMesh(mComponentRegistry);

        MeshLoader2D::load(mComponentRegistry);
        TextureLoader::load(mComponentRegistry);
    }

    void BattleScene::render(int width, int height) {
        const RenderContext context {width, height};

        MeshRenderer2D::draw(mComponentRegistry, context);
        DebugRenderer::draw(mComponentRegistry, context);
        gui_render();
    }

    void BattleScene::update(float dt) {
        gui_update();
        mTerrain.update(mComponentRegistry);
        Physics2D::update(mComponentRegistry, dt);
        PlayerController::move(mComponentRegistry);
    }
}
