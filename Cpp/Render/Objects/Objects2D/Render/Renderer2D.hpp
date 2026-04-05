//
// Created by Stefan on 29.03.2026.
//

#pragma once
#include "RenderObject2D.hpp"
#include "RenderObjectRef2D.hpp"
#include "Render/Shader/Shader.hpp"
#include "Utils/DataStorage/SparseSet.hpp"

namespace Render {
    class Camera;
}

namespace Game {
    class Renderer2D {
        Render::Shader mShader;
        Render::Camera* mCamera = nullptr;

        ggm::SparseSet<Obj2D::RenderObject2D> mObjects;

    public:
        Renderer2D();
        ~Renderer2D();

        void init(Render::Camera* camera);
        void render(int width, int height);

        template<typename... Args>
        Obj2D::RenderObjRef2D newObject(Args &&... args) {
            return {mObjects.push(std::forward<Args>(args)...), mObjects};
        }
    };
} // Game