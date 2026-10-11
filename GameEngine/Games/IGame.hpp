//
// Created by ertls on 26.02.2026.
//

#pragma once
#include "../EngineDefines.hpp"

namespace ggm {
    class CommandRegistry;
}

namespace ggm {
    class Camera;
    class Input;
    class Screen;
}

namespace ggm {

    struct EngineContext {
        Input* input;
        Screen* screen;
        CommandRegistry* commandRegistry;
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
