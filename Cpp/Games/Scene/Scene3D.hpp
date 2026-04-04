//
// Created by Stefan on 04.04.2026.
//

#pragma once

#include "Games/DefaultLayer/PhysicsHandler3D.hpp"
#include "Games/DefaultLayer/Renderer3D.hpp"
#include "Render/Objects/GameObject.hpp"
#include "Render/Objects/Render/RenderObject3D.hpp"
#include "Render/Transformation/Camera.hpp"

namespace Game {
    class Scene3D {
        ggm::InlineVector<Obj3D::GameObject, 16> mGameObjects{};

        PhysicsHandler3D mPhysicsHandler{};
        Renderer3D mRenderLayer{};

        Render::Camera mCamera{};

    public:
        Scene3D();
        Obj3D::GameObject& addObject(const std::string &objFile, float scale, const Obj3D::PhysicsSettings &settings);

        Render::Camera& getCamera();

        void init();

        void render(int width, int height);
        void update(float dt);
    };
} // Game