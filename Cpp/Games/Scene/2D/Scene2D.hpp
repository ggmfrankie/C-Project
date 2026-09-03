//
// Created by Stefan on 05.04.2026.
//

#pragma once
#include "EngineDefines.hpp"
#include "LayerStack.hpp"
#include "Engine/Core/PhysixBox2D/PhysicsSystem.hpp"
#include "Games/Scene/IScene.hpp"
#include "Render/Objects/2D/GameObject.hpp"
#include "Render/Objects/2D/Render/Renderer.hpp"
#include "Render/Transformation/Camera.hpp"


namespace Game2D {
    class Scene2D final : public Game::IScene {
        Render::Camera mCamera{};
        PhysixBox::PhysicsSystem mPhysicsSystem{};
        Renderer mRenderer{};

        ggm::SparseSet<GameObject> mObjects{16};
        LayerStack mLayerStack;

    public:
        Scene2D();
        Scene2D(const Scene2D&) = delete;
        Scene2D& operator=(const Scene2D&) = delete;
        Scene2D(Scene2D&&) = delete;
        Scene2D& operator=(Scene2D&&) = delete;

        void init() override;
        void render(int width, int height) override;
        void update(float dt) override;
        Render::Camera & getCamera() override;

        ggm::u64 addObject(const ggm::Vector3f& color, const PhysicsFactory2D::PhysicsSettings& settings);
        void addLayer(std::unique_ptr<ILayer> layer);
    };
}