//
// Created by Stefan on 11.10.2026.
//

#pragma once
#include "Games/IGame.hpp"
#include "Games/Scene/2D/Scene2D.hpp"
#include "Test/Java.hpp"

namespace ggmTanks {
    class TankGame : public ggm::IGame {
        ggm::Scene2D mScene {};
        ggm::EngineContext mContext {};

        TankGame();
        ~TankGame() override;

    public:
        void preInit(ggm::EngineContext &&e) override;
        void postInit() override;

        void onUpdate(double deltaTime) override;

        void onShutdown() override;
    };
} // ggmTanks
