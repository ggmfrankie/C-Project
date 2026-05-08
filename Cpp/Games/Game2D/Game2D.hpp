//
// Created by Stefan on 05.04.2026.
//

#pragma once
#include "../EngineDefines.hpp"
#include "Games/IGame.hpp"
#include "Games/Scene/2D/Scene2D.hpp"

namespace Game2D {
    class Game2D final : public Game::IGame{
        Scene2D mScene{};

        Render::Camera* mCamera = nullptr;
        Render::Input* mInput = nullptr;
        Render::Screen* mScreen = nullptr;
        Engine::CommandRegistry* mCommandRegistry = nullptr;

    public:
        /// @brief Use this to create Objects. OpenGL is NOT initialized here
        void preInit(Game::EngineContext &&e) override;

        void postInit() override;

        void onShutdown() override;

        void onUpdate(double deltaTime) override;
    };
} // Game