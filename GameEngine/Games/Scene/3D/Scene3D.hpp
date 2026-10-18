//
// Created by Stefan on 04.04.2026.
//

#pragma once

#include "../IScene.hpp"
#include "Render/Objects/3D/GameObject3D.hpp"
#include "Render/Objects/3D/Render/Renderer3D.hpp"
#include "Render/Transformation/Camera.hpp"
#include "Render/Objects/3D/Physics/PhysicsFactory3D.hpp"
#include "Render/Objects/3D/Physics/PhysicsHandler3D.hpp"

namespace ggm {
    class Scene3D final : public IScene {
        ArrayList<GameObject3D, 64> mGameObjects{};

        PhysicsHandler3D mPhysicsHandler{};
        Renderer3D mRenderer{};

        Camera mCamera{};

    public:
        Scene3D();
        GameObject3D& addObject(const std::filesystem::path& objFile, float scale, const ggm::PhysicsSettings &settings);

        void init() override;

        void render(int width, int height) override;
        void update(float dt) override;

        Camera& getCamera();
    };
} // Game