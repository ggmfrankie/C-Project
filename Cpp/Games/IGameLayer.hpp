//
// Created by ertls on 13.03.2026.
//
#pragma once

namespace Render {
    class Input;
    class Camera;
}

namespace Engine {
    class Scene;
}

namespace Game{
    struct LayerEngineContext
    {
        Render::Input* input;
        Render::Camera* camera;
        Engine::Scene* scene;
    };

    class IGameLayer {
        public:
        virtual ~IGameLayer() = default;

        virtual void onAttach(const LayerEngineContext &ec) = 0;
        virtual void onDetach() = 0;

        virtual void onUpdate(float dt) = 0;
        virtual void onRender(int width, int height) = 0;

        bool enabled = false;
    };
}
