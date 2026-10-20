//
// Created by Stefan on 11.10.2026.
//

#pragma once
#include "BattleScene.hpp"
#include "Games/IGame.hpp"
#include "Test/Java.hpp"

namespace ggmTanks {
    class TankGame : public ggm::IGame {
        BattleScene mScene {};
        ggm::EngineContext mContext {};
    public:
        TankGame();
        ~TankGame() override;

        void preInit(ggm::EngineContext &&e) override;
        void postInit() override;

        void onUpdate(double deltaTime) override;

        void onShutdown() override;
    };
} // ggmTanks
