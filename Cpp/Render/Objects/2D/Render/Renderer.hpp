//
// Created by Stefan on 29.03.2026.
//

#pragma once
#include "../EngineDefines.hpp"
#include "RenderObject.hpp"
#include "RenderObjectRef.hpp"
#include "Render/Shader/Shader.hpp"
#include "Utils/DataStorage/SparseSet.hpp"

namespace Render {
    class Camera;
}

namespace Game2D {
    class Renderer {
        Render::Shader mShader;
        Render::Camera* mCamera = nullptr;

        ggm::SparseSet<RenderObject> mObjects;

    public:
        Renderer();
        ~Renderer();

        void init(Render::Camera* camera);
        void render(int width, int height);

        template<typename... Args>
        RenderObjRef2D newObject(Args &&... args) {
            return {mObjects.push(std::forward<Args>(args)...), mObjects};
        }
    };
} // Game