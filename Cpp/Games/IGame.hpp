//
// Created by ertls on 26.02.2026.
//

#pragma once
#include <vector>

#include "../Render/IO/Input.hpp"
#include "../Render/Objects/Render/RenderObject.hpp"
#include "../Render/Objects/Loader/OBJLoader.hpp"
#include "../Render/Transformation/Camera.hpp"
#include "Engine/CommandRegistry.hpp"
#include "Render/Screen.hpp"


namespace Game {

    struct EngineContext {
        Render::Input& input;
        Render::Camera& camera;
        Render::Screen& screen;
        Engine::CommandRegistry& commandRegistry;
    };

    class IGame {
    public:
        virtual ~IGame() = default;

        virtual void preInit() = 0;

        virtual void postInit() = 0;

        virtual void onShutdown() = 0;

        virtual void onUpdate(double deltaTime) = 0;

        virtual void passState(EngineContext&& e) = 0;
    };
} // ggm
