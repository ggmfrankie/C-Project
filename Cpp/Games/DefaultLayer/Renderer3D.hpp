//
// Created by ertls on 13.03.2026.
//

#pragma once

#include "Render/Objects/Render/RenderObject3D.hpp"
#include "Utils/DataStorage/SparseSet.hpp"

namespace Render {
    class Camera;
}

namespace Obj3D {
    struct RenderObjectRef {
        ggm::SparseSet<RenderObject3D>& interface;
        ggm::u64 id;

        [[nodiscard]] RenderObject3D& get() const {
            return interface.get(id);
        }
    };
}
namespace Game {

    class Renderer3D  {

        Render::Shader mShader;
        Render::Camera* mCamera = nullptr;

        ggm::SparseSet<Obj3D::RenderObject3D> mObjects;

    public:
        Renderer3D();
        ~Renderer3D();

        void init(Render::Camera* camera);
        void render(int width, int height);

        template<typename... Args>
        Obj3D::RenderObjectRef newObject(Args &&... args) {
            return { mObjects, mObjects.push(std::forward<Args>(args)...) };
        }
    };
} // Game