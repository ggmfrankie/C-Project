//
// Created by Stefan on 15.03.2026.
//

#pragma once
#include "Render/Objects/GameObject.hpp"
#include "Render/Transformation/Camera.hpp"

namespace Engine {
    class Scene {
        std::vector<Obj::GameObject> mObjects;
        Render::Camera camera;
    public:
        Scene();

        template<typename... Args>
        Obj::GameObject& pushObject(Args&&... args) {
            mObjects.emplace_back(std::forward<Args>(args)...);
            return mObjects.back();
        }

        void init();

        Render::Camera &getCamera() { return camera; }
        std::vector<Obj::GameObject>& getObjects() { return mObjects; }
    };
} // Engine