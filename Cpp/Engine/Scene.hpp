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

        void pushObject(Obj::GameObject&& obj);

        void init();

        Render::Camera &getCamera() { return camera; }
        std::vector<Obj::GameObject>& getObjects() { return mObjects; }
    };
} // Engine