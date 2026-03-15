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
        mShader.createUniform("textureSampler");
        mShader.createUniform("modelViewMatrix");
        mShader.createUniform("projectionMatrix");
        mShader.init();
        mShader.link();
        mCamera = ec.camera;
        mInput = ec.input;
        mScene = ec.scene;
    }

    void RenderLayer::onDetach() {
    }

    void RenderLayer::onUpdate(float dt) {
    }

    void RenderLayer::onRender(const int width, const int height) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        //glDisable(GL_CULL_FACE);

        mShader.bind();

        mShader.setUniform("projectionMatrix",
                              ggm::Matrix4f::Perspective(
                                  mCamera->getFOV(),
                                  static_cast<float>(width) / static_cast<float>(height),
                                  0.01f,
                                  1000.0f
                                  )
                             );

        const ggm::Matrix4f view = mCamera->getViewMatrix();

        for (auto& obj : mGameObjects) {

            ggm::Matrix4f& model = obj.getRenderObject().getModelMatrix();
            ggm::Matrix4f modelView = view * model;

            mShader.setUniform("modelViewMatrix", modelView);

            obj.getRenderObject().render();
        }
        gui_render();

    }
} // Game