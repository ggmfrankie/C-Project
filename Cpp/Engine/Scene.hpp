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

        std::vector<Obj::GameObject>& getObjects() { return mObjects; }
        void pushObject(Obj::GameObject&& obj);

        Render::Camera &getCamera() { return camera; }
    };
} // Engine