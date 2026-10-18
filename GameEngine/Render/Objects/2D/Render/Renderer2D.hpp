//
// Created by Stefan on 29.03.2026.
//

#pragma once
#include "EngineDefines.hpp"
#include "RenderObject2D.hpp"
#include "RenderObjectRef2D.hpp"
#include "Render/Shader/Shader.hpp"
#include "Utils/DataStorage/SparseSet.hpp"

namespace ggm {
    class Camera;

    class Renderer2D {
        Shader mShader;
        SparseSet<RenderObject2D> mObjects;

    public:
        Renderer2D();
        ~Renderer2D();

        void init();
        void render(int width, int height);

        template<typename... Args>
        RenderObjRef2D newObject(Args &&... args) {
            return {mObjects.push(std::forward<Args>(args)...), mObjects};
        }
    };
} // Game