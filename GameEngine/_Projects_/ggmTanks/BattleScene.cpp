//
// Created by Stefan on 18.10.2026.
//
#include "BattleScene.hpp"

#include "Engine/Core/EntityComponentSystem/Core/RenderContext.hpp"
#include "Engine/Core/EntityComponentSystem/Systems/Physics/PlayerController.hpp"
#include "Engine/Core/EntityComponentSystem/Systems/Render/MeshRenderer2D.hpp"
#include "Engine/Core/EntityComponentSystem/Systems/Loader/MeshLoader2D.hpp"
#include "Engine/Core/EntityComponentSystem/Systems/Loader/TextureLoader.hpp"

namespace ggmTanks {
    using namespace ggm;
    BattleScene::BattleScene() :
        mTerrain(400, 200)
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
            PlayerControlled{}
        );

        //mTerrain.registerMesh(mComponentRegistry);

        MeshLoader2D::load(mComponentRegistry);
        TextureLoader::load(mComponentRegistry);
    }

    void BattleScene::render(int width, int height) {
        const RenderContext context {width, height};

        MeshRenderer2D::draw(mComponentRegistry, context);
    }

    void BattleScene::update(float dt) {
        mTerrain.update(mComponentRegistry);
        PlayerController::move(mComponentRegistry);
    }
}
