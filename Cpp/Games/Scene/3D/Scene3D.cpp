//
// Created by Stefan on 04.04.2026.
//

#include "Scene3D.hpp"
#include "../../../Render/Objects/Render/Renderer3D.hpp"
#include "Render/Objects/Physics/PhysicsFactory.hpp"

namespace Game {
    using namespace Obj3D;

    Scene3D::Scene3D() {
        mPhysicsHandler.init();
    }

    GameObject& Scene3D::addObject(const std::string& objFile, float scale, const PhysicsSettings& settings) {
        auto ro = mRenderLayer.newObject(objFile);
        ro.get().scaleTo(scale);
        auto po = mPhysicsHandler.newObj(settings);

        mGameObjects.push(ro, po);
        return mGameObjects.back();
    }

    Render::Camera& Scene3D::getCamera() {
        return mCamera;
    }

    void Scene3D::init() {
        mRenderLayer.init(&mCamera);
    }

    void Scene3D::render(int width, int height) {
        mRenderLayer.render(width, height);
    }

    void Scene3D::update(float dt) {
        for (auto& obj : mGameObjects) {
            obj.sync();
        }
        mPhysicsHandler.update(dt);
    }
} // Game