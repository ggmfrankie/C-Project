//
// Created by ertls on 13.03.2026.
//

#pragma once
#include <vector>

#include "Engine/Scene.hpp"
#include "Games/IGame.hpp"
#include "Games/IGameLayer.hpp"
#include "Render/IO/Input.hpp"
#include "Render/Objects/GameObject.hpp"
#include "Render/Transformation/Camera.hpp"


namespace Game {
    class RenderLayer final : public IGameLayer {

        std::vector<Obj::GameObject> mGameObjects;
        Render::Shader mShader;
        Render::Camera* mCamera = nullptr;
        Render::Input* mInput = nullptr;
        Engine::Scene* mScene = nullptr;

    public:
        RenderLayer();
        ~RenderLayer() override;

        void onAttach(const LayerEngineContext &ec) override;
        void onDetach() override;

        void onUpdate(float dt) override;

        void onRender(int width, int height) override;
    };
} // Game