//
// Created by ertls on 13.03.2026.
//

#include "RenderLayer.hpp"

#include "GuiInterface.h"
#include "GLFW/glfw3.h"

namespace Game {
    RenderLayer::RenderLayer() :
        mShader("MainShader.vert", "MainShader.frag")
    {
    }

    RenderLayer::~RenderLayer() = default;

    void RenderLayer::onAttach(const LayerEngineContext& ec) {
        mCamera = ec.camera;
        mInput = ec.input;
        mScene = ec.scene;
    }

    void RenderLayer::onInit() {
        mShader.init();
        mShader.link();

        mShader.createUniform("textureSampler");
        mShader.createUniform("modelViewMatrix");
        mShader.createUniform("projectionMatrix");
    }

    void RenderLayer::onDetach() {
    }

    void RenderLayer::onUpdate(float dt) {
        for (auto& obj : mScene->getObjects()) {
            obj.sync();
        }
    }

    void RenderLayer::onRender(const int width, const int height) {
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        mShader.bind();
        mShader.setUniform("textureSampler", 0);
        mShader.setUniform("projectionMatrix",
                              ggm::Matrix4f::Perspective(
                                  mCamera->getFOV(),
                                  static_cast<float>(width) / static_cast<float>(height),
                                  0.01f,
                                  1000.0f
                                  )
                             );

        const ggm::Matrix4f view = mCamera->getViewMatrix();

        for (auto& obj : mScene->getObjects()) {
            auto& renderObj = obj.getRenderObject();

            const ggm::Matrix4f& model = renderObj.getModelMatrix();
            ggm::Matrix4f modelView = view * model;

            mShader.setUniform("modelViewMatrix", modelView);

            renderObj.render();
        }
        gui_render();
    }
} // Game