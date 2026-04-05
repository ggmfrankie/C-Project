//
// Created by Stefan on 05.04.2026.
//

#pragma once
#include "Engine/Core/PhysixBox2D/PhysicsSystem.hpp"
#include "Games/Scene/IScene.hpp"
#include "Render/Objects/Objects2D/GameObject2D.hpp"
#include "Render/Objects/Objects2D/Render/Renderer2D.hpp"
#include "Render/Transformation/Camera.hpp"


namespace Game {
    class Scene2D final : public IScene {
        Render::Camera mCamera{};
        PhysixBox::PhysicsSystem mPhysicsSystem{};
        Renderer2D mRenderer{};

        ggm::InlineVector<Obj2D::GameObject2D, 16> mObjects{};

    public:
        Scene2D();

        void init() override;
        void render(int width, int height) override;
        void update(float dt) override;
        Render::Camera & getCamera() override;

        Obj2D::GameObject2D& addObject(const ggm::Vector3f& color, const Obj2D::PhysicsFactory2D::PhysicsSettings& settings);
    };
} // Game