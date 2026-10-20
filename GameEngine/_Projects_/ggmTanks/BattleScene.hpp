//
// Created by Stefan on 18.10.2026.
//

#pragma once
#include "Registry.hpp"
#include "Engine/Core/EntityComponentSystem/ComponentRegistry.hpp"
#include "Games/Scene/IScene.hpp"

namespace ggmTanks {
    class BattleScene : public ggm::IScene {
        Registry mComponentRegistry {};

    public:
        void init() override;

        void render(int width, int height) override;
        void update(float dt) override;
    };
} // ggm
