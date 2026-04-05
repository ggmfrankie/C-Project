//
// Created by Stefan on 29.03.2026.
//

#include "Renderer2D.hpp"
#include "Render/Objects/Objects2D/Render/RenderObject2D.hpp"

namespace Game {
    Renderer2D::Renderer2D() :
        mShader("Shader2D.vert", "Shader2D.frag"),
        mObjects(64)
    {
    }

    Renderer2D::~Renderer2D() = default;

    void Renderer2D::init(Render::Camera* camera) {
        mShader.init();
        mShader.link();

        mShader.createUniform("textureSampler");
        mShader.createUniform("projectionMatrix");

        mCamera = camera;

        for (auto& obj: mObjects) {
            obj.init();
        }
    }

    void Renderer2D::render(int width, int height) {
        for (auto& obj: mObjects) {
            obj.render();
        }
    }
} // Game