//
// Created by Stefan on 04.04.2026.
//

#include "Scene3D.hpp"

#include "GuiInterface.h"
#include "Render/Objects/3D/Render/Renderer3D.hpp"
#include "Render/Objects/3D/Physics/PhysicsFactory.hpp"

namespace Game {
    using namespace Obj3D;
    namespace fs = std::filesystem;

    Scene3D::Scene3D() {
        mPhysicsHandler.init();
    }

    //! @brief Creates a RenderObject from an .obj file and creates a PhysicsObject from the settings given. Creates a Game Object that reference both.
    GameObject& Scene3D::addObject(const fs::path& objFile, float scale, const PhysicsSettings& settings) {
        auto ro = mRenderer.newObject(objFile);
        ro.get().scaleTo(scale);
        auto po = mPhysicsHandler.newObject(settings);

        mGameObjects.push(ro, po);
        return mGameObjects.back();
    }

    Render::Camera& Scene3D::getCamera() {
        return mCamera;
    }

    void Scene3D::init() {
        mRenderer.init(&mCamera);
    }

    void Scene3D::render(int width, int height) {
        mRenderer.render(width, height);
        gui_render();
    }

    void Scene3D::update(float dt) {
        for (auto& obj : mGameObjects) {
            obj.sync();
        }
        mPhysicsHandler.update(dt);
    }
}