//
// Created by ertls on 13.03.2026.
//

#include "RenderLayer3D.hpp"

#include "GuiInterface.h"
#include "GLFW/glfw3.h"

namespace Game {
    RenderLayer3D::RenderLayer3D() : mShader("MainShader.vert", "MainShader.frag"), mObjects(64) {
    }

    RenderLayer3D::~RenderLayer3D() = default;

    void RenderLayer3D::onAttach(const LayerEngineContext& ec) {
        mCamera = ec.camera;
        mInput = ec.input;
        mScene = ec.scene;
    }

    void RenderLayer3D::onInit() {
        mShader.init();
        mShader.link();

        mShader.createUniform("textureSampler");
        mShader.createUniform("modelViewMatrix");
        mShader.createUniform("projectionMatrix");

        for (auto& obj : mObjects) {
            obj.init();
        }
    }

    void RenderLayer3D::onDetach() {
    }

    void RenderLayer3D::onUpdate(float dt) {
        for (auto& obj : mScene->getObjects()) {
            obj.sync();
        }
    }

    void RenderLayer3D::onRender(const int width, const int height) {
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

        for (auto& obj : mObjects) {


            const ggm::Matrix4f& model = obj.getModelMatrix();
            ggm::Matrix4f modelView = view * model;

            mShader.setUniform("modelViewMatrix", modelView);

            obj.render();
        }
        gui_render();
    }
} // Game