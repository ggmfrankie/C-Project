//
// Created by Stefan on 18.10.2026.
//

#pragma once
#include "Engine/Core/EntityComponentSystem/ComponentRegistry.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Position.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Sprite.hpp"
#include "Engine/Core/EntityComponentSystem/Components/SpriteLocation.hpp"
#include "Games/Scene/IScene.hpp"

namespace ggm {
    class BattleScene : public IScene {
        ComponentRegistry<
            Sprite,
            SpriteLocation,
            Position
        > mComponentRegistry {};

    public:
        void init() override;

        void render(int width, int height) override;

        void update(float dt) override;

        Camera& getCamera() override;
    };
} // ggm
