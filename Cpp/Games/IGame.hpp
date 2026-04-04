//
// Created by ertls on 26.02.2026.
//

#pragma once


namespace Engine {
    class CommandRegistry;
}

namespace Render {
    class Camera;
    class Input;
    class Screen;
}

namespace Game {

    struct EngineContext {
        Render::Input& input;
        Render::Screen& screen;
        Engine::CommandRegistry& commandRegistry;
    };

    class IGame {
    public:
        virtual ~IGame() = default;

        virtual void preInit(EngineContext &&e) = 0;

        virtual void postInit() = 0;

        virtual void onShutdown() = 0;

        virtual void onUpdate(double deltaTime) = 0;
    };
} // ggm
