//
// Created by Stefan on 06.05.2026.
//

#pragma once
#include "Games/Scene/2D/ILayer.hpp"
#include "Render/Shader/Shader.hpp"

namespace Game2D {

class DebugLayer : public ILayer {
    Render::Shader mShader;
    ggm::SparseSet<GameObject>* mObjects = nullptr;
public:
    DebugLayer();
    void onUpdate(float dt) override;

    void onRender(int width, int height) override;

    void onInit() override;

    void onAttach(ggm::SparseSet<GameObject>& data) override;
};

}
