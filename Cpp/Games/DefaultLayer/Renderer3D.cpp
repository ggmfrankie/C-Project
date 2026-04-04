//
// Created by ertls on 13.03.2026.
//

#include "Renderer3D.hpp"

#include "GuiInterface.h"
#include "GLFW/glfw3.h"
#include "Render/Transformation/Camera.hpp"

namespace Game {
    Renderer3D::Renderer3D() : mShader("MainShader.vert", "MainShader.frag"), mObjects(64) {
    }

    Renderer3D::~Renderer3D() = default;

    void Renderer3D::init(Render::Camera* camera) {
        mShader.init();
        mShader.link();

        mShader.createUniform("textureSampler");
        mShader.createUniform("modelViewMatrix");
        mShader.createUniform("projectionMatrix");

        mCamera = camera;

        for (auto& obj : mObjects) {
            obj.init();
        }
    }

    void Renderer3D::render(const int width, const int height) {
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