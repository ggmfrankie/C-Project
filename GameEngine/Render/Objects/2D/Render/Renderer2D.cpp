//
// Created by Stefan on 29.03.2026.
//

#include "Renderer2D.hpp"
#include "Render/Objects/2D/Render/RenderObject2D.hpp"

namespace ggm {
    Renderer2D::Renderer2D() :
        mShader("2D/Shader2D.vert", "2D/Shader2D.frag"),
        mObjects(64)
    {}

    Renderer2D::~Renderer2D() = default;

    void Renderer2D::init(Camera* camera) {
        mShader.compileAndLink();
        mCamera = camera;

        for (auto& obj: mObjects) {
            obj.init();
        }
    }

    void Renderer2D::render(int width, int height) {
        glDisable(GL_DEPTH_TEST);
        mShader.bind();
        mShader.setUniform("screenWidth", static_cast<float>(width));
        mShader.setUniform("screenHeight", static_cast<float>(height));
        for (auto& obj: mObjects) {
            mShader.setUniform("color", obj.getColor());
            obj.render();
        }
        mShader.unbind();
        glEnable(GL_DEPTH_TEST);
    }
} // Game