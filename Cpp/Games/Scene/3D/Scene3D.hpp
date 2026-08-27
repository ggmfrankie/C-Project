//
// Created by Stefan on 04.04.2026.
//

#pragma once

#include "../IScene.hpp"
#include "Render/Objects/3D/GameObject.hpp"
#include "Render/Objects/3D/Render/Renderer3D.hpp"
#include "Render/Transformation/Camera.hpp"
#include "Render/Objects/3D/Physics/PhysicsFactory.hpp"
#include "Render/Objects/3D/Physics/PhysicsHandler3D.hpp"

namespace Game {
    class Scene3D final : public IScene {
        ggm::InlineVector<Obj3D::GameObject, 64> mGameObjects{};

        PhysicsHandler3D mPhysicsHandler{};
        Renderer3D mRenderer{};

        Render::Camera mCamera{};

    public:
        Scene3D();
        Obj3D::GameObject& addObject(const std::filesystem::path& objFile, float scale, const Obj3D::PhysicsSettings &settings);

        void init() override;

        void render(int width, int height) override;
        void update(float dt) override;

        Render::Camera& getCamera() override;
    };
} // Game