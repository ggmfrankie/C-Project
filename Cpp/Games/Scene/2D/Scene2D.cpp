//
// Created by Stefan on 05.04.2026.
//

#include "Scene2D.hpp"
#include "Render/Objects/Objects2D/Physics/PhysicsFactory2D.hpp"

namespace Game {
    using namespace Obj2D;
    using namespace Obj2D::PhysicsFactory2D;
    Scene2D::Scene2D() = default;

    void Scene2D::init() {
        mRenderer.init(&mCamera);
    }

    void Scene2D::render(int width, int height) {
        mRenderer.render(width, height);
    }

    void Scene2D::update(float dt) {
        mPhysicsSystem.update(1, dt);
    }

    Render::Camera& Scene2D::getCamera() {
        return mCamera;
    }

    GameObject2D& Scene2D::addObject(const ggm::Vector3f &color, const PhysicsSettings &settings) {
        auto ro = mRenderer.newObject(color, settings.points);
        auto po = mPhysicsSystem.addBody(settings);

        return mObjects.push(ro, po);
    }
} // Game