//
// Created by ertls on 26.02.2026.
//

#ifndef MIXEDPROJECT_BASEGAME_H
#define MIXEDPROJECT_BASEGAME_H
#include "../IGame.hpp"

namespace Game {
    class BaseGame: public IGame{

    public:
        BaseGame() = default;
        ~BaseGame() override = default;

        void preInit() override;
        void postInit() override;

        void onUpdate(double deltaTime) override;
        void onShutdown() override;

        void passState(EngineContext &&e) override;



    private:
        Render::Camera* camera = nullptr;
        Render::Input* input = nullptr;
        Render::Screen* screen = nullptr;
        Engine::CommandRegistry* cRegistry = nullptr;

        bool guiMode = false;

        float CAMERA_POS_STEP = 0.03f;
        static constexpr float MOUSE_SENSITIVITY = 0.05f;

        static void toggleCursorMode(GLFWwindow *window, bool guiMode);
    };
} // Render

#endif //MIXEDPROJECT_BASEGAME_H