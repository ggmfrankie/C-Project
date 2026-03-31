//
// Created by Stefan on 15.03.2026.
//

#include "Scene.hpp"

namespace Engine {
    Scene::Scene(Render::Input& input) {
        mObjects.reserve(16);
        mLayerStack.onAttach({&input, &mCamera, this});
    }

    void Scene::render(int width, int height) const {
        mLayerStack.render(width, height);
    }

    void Scene::update(float dt) {
        mLayerStack.update(dt);
    }

    void Scene::init() {
        mLayerStack.init();
    }
} // Engine