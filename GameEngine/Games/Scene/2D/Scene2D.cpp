//
// Created by Stefan on 05.04.2026.
//

#include "Scene2D.hpp"

#include "GuiInterface.h"
#include "Render/Objects/2D/Physics/PhysicsFactory.hpp"

namespace Game2D {
    Scene2D::Scene2D() :
        mLayerStack(mObjects)
    {}

    void Scene2D::init() {
        mRenderer.init(&mCamera);
        mLayerStack.initAll();
    }

    void Scene2D::render(int width, int height) {
        mRenderer.render(width, height);
        mLayerStack.renderAll(width, height);
        gui_render();
    }

    void Scene2D::update(float dt) {
        mPhysicsSystem.update(10, dt);
        for (auto& obj : mObjects) {
            obj.update();
        }
        mLayerStack.updateAll(dt);
    }

    Render::Camera& Scene2D::getCamera() {
        return mCamera;
    }

    ggm::u64 Scene2D::addObject(const ggm::Vector3f &color, const PhysicsFactory2D::PhysicsSettings &settings) {
        auto ro = mRenderer.newObject(color, settings.points);
        auto po = mPhysicsSystem.addBody(settings);

        return mObjects.push(ro, po);
    }

    void Scene2D::addLayer(std::unique_ptr<ILayer> layer) {
        mLayerStack.pushLayer(std::move(layer));
    }
}