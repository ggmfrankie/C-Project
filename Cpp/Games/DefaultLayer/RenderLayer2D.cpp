//
// Created by Stefan on 29.03.2026.
//

#include "RenderLayer2D.hpp"

namespace Game {
    RenderLayer2D::RenderLayer2D() :
        mShader("Shader2D.vert", "Shader2D.frag"),
        mObjects(64)
    {
    }

    RenderLayer2D::~RenderLayer2D() = default;

    void RenderLayer2D::onAttach(const LayerEngineContext &ec) {
        mInput = ec.input;
    }

    void RenderLayer2D::onDetach() {
    }

    void RenderLayer2D::onInit() {
        mShader.init();
        mShader.link();

        mShader.createUniform("textureSampler");
        mShader.createUniform("projectionMatrix");

        for (auto& obj: mObjects) {
            obj.init();
        }
    }

    void RenderLayer2D::onUpdate(float dt) {
    }

    void RenderLayer2D::onRender(int width, int height) {
        for (auto& obj: mObjects) {
            obj.render();
        }
    }
} // Game