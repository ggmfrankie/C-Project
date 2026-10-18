//
// Created by Stefan on 05.04.2026.
//

#pragma once
#include "EngineDefines.hpp"
#include "LayerStack.hpp"
#include "Engine/Core/PhysixBox2D/PhysicsSystem.hpp"
#include "Games/Scene/IScene.hpp"
#include "Render/Objects/2D/GameObject2D.hpp"
#include "Render/Objects/2D/Render/Renderer2D.hpp"
#include "Render/Transformation/Camera.hpp"


namespace ggm {
    class Scene2D final : public IScene {
        PhysixBox::PhysicsSystem mPhysicsSystem{};
        Renderer2D mRenderer{};

        SparseSet<GameObject2D> mObjects{16};
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

        u64 addObject(const Vector3f& color, const PhysicsFactory2D::PhysicsSettings& settings);
        void addLayer(std::unique_ptr<ILayer> layer);
    };
}