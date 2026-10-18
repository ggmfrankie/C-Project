//
// Created by Stefan on 05.04.2026.
//

#pragma once
#include "../EngineDefines.hpp"
#include "Games/IGame.hpp"
#include "Games/Scene/2D/Scene2D.hpp"

namespace ggm {
    class Game2D : public IGame{
        Scene2D mScene{};

        Camera* mCamera = nullptr;
        Input* mInput = nullptr;
        Screen* mScreen = nullptr;
        CommandRegistry* mCommandRegistry = nullptr;

    public:
        /// @brief Use this to create Objects. OpenGL is NOT initialized here
        void preInit(EngineContext &&e) override;

        void postInit() override;

        void onShutdown() override;

        void onUpdate(double deltaTime) override;
    };
} // Game