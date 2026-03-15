//
// Created by Stefan on 15.03.2026.
//

#include "Scene.hpp"

namespace Engine {
    Scene::Scene() {
        mObjects.reserve(16);
    }

    void Scene::pushObject(Obj::GameObject &&obj) {
        mObjects.push_back(std::move(obj));
    }
} // Engine