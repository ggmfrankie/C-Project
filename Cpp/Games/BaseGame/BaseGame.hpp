//
// Created by ertls on 26.02.2026.
//

#ifndef MIXEDPROJECT_BASEGAME_H
#define MIXEDPROJECT_BASEGAME_H
#include "../IGame.hpp"

#include "Games/Scene/3D/Scene3D.hpp"

struct GLFWwindow {
};


namespace Game {
    class Scene3D;
    class Player;

    class BaseGame final : public IGame{

    public:
        BaseGame();
        ~BaseGame() override;

        void preInit(EngineContext &&e) override;
        void postInit() override;

        void onUpdate(double deltaTime) override;
        void onShutdown() override;

    private:
        Render::Camera* mCamera = nullptr;
        Render::Input* mInput = nullptr;
        Render::Screen* mScreen = nullptr;
        Engine::CommandRegistry* mCommandRegistry = nullptr;

        Scene3D mScene;

        bool guiMode = false;

        float CAMERA_POS_STEP = 0.03f;
        static constexpr float MOUSE_SENSITIVITY = 0.05f;

        static void toggleCursorMode(GLFWwindow *window, bool guiMode);
    };
} // Render

#endif //MIXEDPROJECT_BASEGAME_H