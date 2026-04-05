//
// Created by Stefan on 05.04.2026.
//

#pragma once

namespace Render {
    class Camera;
}

namespace Game {
    class IScene {
    public:
        virtual void init() = 0;

        virtual void render(int width, int height) = 0;
        virtual void update(float dt) = 0;

        virtual Render::Camera& getCamera() = 0;

        virtual ~IScene() = default;
    };
} // Game