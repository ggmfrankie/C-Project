//
// Created by Stefan on 29.03.2026.
//

#include "Renderer.hpp"
#include "Render/Objects/2D/Render/RenderObject.hpp"

namespace Game2D {
    Renderer::Renderer() :
        mShader("2D/Shader2D.vert", "2D/Shader2D.frag"),
        mObjects(64)
    {}

    Renderer::~Renderer() = default;

    void Renderer::init(Render::Camera* camera) {
        mShader.compileAndLink();
        mCamera = camera;

        for (auto& obj: mObjects) {
            obj.init();
        }
    }

    void Renderer::render(int width, int height) {
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