//
// Created by ertls on 13.03.2026.
//

#pragma once

#include "../Scene/Scene.hpp"
#include "Games/IGameLayer.hpp"
#include "Render/IO/Input.hpp"
#include "Render/Objects/GameObject.hpp"
#include "Render/Transformation/Camera.hpp"
#include "Utils/DataStorage/SparseSet.hpp"


namespace Game {
    class RenderLayer3D final : public IGameLayer {

        Render::Shader mShader;
        Render::Camera* mCamera = nullptr;
        Render::Input* mInput = nullptr;
        Engine::Scene* mScene = nullptr;

        ggm::SparseSet<Obj::RenderObject> mObjects;

    public:
        RenderLayer3D();
        ~RenderLayer3D() override;

        void onAttach(const LayerEngineContext &ec) override;
        void onInit() override;
        void onDetach() override;

        void onUpdate(float dt) override;

        void onRender(int width, int height) override;

        template<typename... Args>
        auto newObject(Args &&... args) -> std::pair<ggm::SparseSet<Obj::RenderObject>&, ggm::i64> {
            return { mObjects, static_cast<ggm::u64>(mObjects.push(std::forward<Args>(args)...)) };
        }
    };
} // Game