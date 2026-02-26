//
// Created by ertls on 26.02.2026.
//

#ifndef MIXEDPROJECT_BASEGAME_H
#define MIXEDPROJECT_BASEGAME_H
#include "IGame.h"

namespace Render {
    class BaseGame: public IGame{

    public:
        BaseGame() = default;
        ~BaseGame() override = default;

        void onInit() override;
        void onUpdate(float deltaTime) override;
        void onShutdown() override;

        void passState(EngineContext &&e) override;

    private:
        Camera* camera = nullptr;
        Input* input = nullptr;
        std::vector<Obj::Object>* objects = nullptr;

        static constexpr float CAMERA_POS_STEP = 0.03f;
        static constexpr float MOUSE_SENSITIVITY = 0.0005f;
    };
} // Render

#endif //MIXEDPROJECT_BASEGAME_H