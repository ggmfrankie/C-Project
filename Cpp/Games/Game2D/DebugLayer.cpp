//
// Created by Stefan on 06.05.2026.
//

#include "DebugLayer.hpp"

#include "Render/Objects/Objects2D/Render/Renderer.hpp"

namespace Game2D {

    // TODO: FIX
    DebugLayer::DebugLayer() : mShader("2D/Shader2D.vert", "2D/Shader2D.frag")
    {}

    void DebugLayer::onUpdate(float dt) {}

    void DebugLayer::onRender(int width, int height) {
        glDisable(GL_DEPTH_TEST);
        mShader.bind();
        mShader.setUniform("screenWidth", static_cast<float>(width));
        mShader.setUniform("screenHeight", static_cast<float>(height));
        mShader.setUniform("color", ggm::Vector3f{});
        glPointSize(10.0f);
        for (auto& object: *mObjects) {
            auto& rO = object.getRenderObj();
            auto[vao, vbos, vCount] = rO.getMesh().getGlData();

            glBindVertexArray(vao);
            glDrawArrays(GL_POINTS, 0, vCount);
            glBindVertexArray(0);
        }
        mShader.unbind();
        glEnable(GL_DEPTH_TEST);
    }

    void DebugLayer::onInit() {
        mShader.compile();
        mShader.link();

        mShader.createUniform("color");
        mShader.createUniform("screenWidth");
        mShader.createUniform("screenHeight");
    }

    void DebugLayer::onAttach(ggm::SparseSet<GameObject>& data) {
        mObjects = &data;
    }

}