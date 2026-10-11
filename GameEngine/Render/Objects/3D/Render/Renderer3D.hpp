//
// Created by ertls on 13.03.2026.
//

#pragma once
#include "EngineDefines.hpp"
#include "RenderObjectRef3D.hpp"
#include "Render/Shader/Shader.hpp"
#include "Utils/DataStorage/SparseSet.hpp"

namespace ggm {
    class Camera;
}

namespace ggm {

    class Renderer3D  {

        ggm::Shader mShader;
        ggm::Camera* mCamera = nullptr;

        ggm::SparseSet<ggm::RenderObject3D> mObjects;

    public:
        Renderer3D();
        ~Renderer3D();

        void init(ggm::Camera* camera);
        void render(int width, int height);

        template<typename... Args>
        ggm::RenderObjectRef newObject(Args &&... args) {
            return {mObjects.push(std::forward<Args>(args)...) , mObjects};
        }
    };
} // Game