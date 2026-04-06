//
// Created by Stefan on 29.03.2026.
//

#include "Renderer2D.hpp"
#include "Render/Objects/Objects2D/Render/RenderObject2D.hpp"

namespace Game {
    Renderer2D::Renderer2D() :
        mShader("2D/Shader2D.vert", "2D/Shader2D.frag"),
        mObjects(64)
    {
    }

    Renderer2D::~Renderer2D() = default;

    void Renderer2D::init(Render::Camera* camera) {
        mShader.compile();
        mShader.link();

        mShader.createUniform("color");
        mShader.createUniform("screenWidth");
        mShader.createUniform("screenHeight");

        mCamera = camera;

        for (auto& obj: mObjects) {
            puts("init objects");
            obj.init();
        }
    }

    void Renderer2D::render(int width, int height) {
        glDisable(GL_DEPTH_TEST);
        mShader.bind();
        for (auto& obj: mObjects) {
            mShader.setUniform("screenWidth", static_cast<float>(width));
            mShader.setUniform("screenHeight", static_cast<float>(height));
            mShader.setUniform("color", obj.getColor());
            obj.render();
        }
        mShader.unbind();
        glEnable(GL_DEPTH_TEST);
    }
} // Game