//
// Created by Stefan on 06.05.2026.
//

#pragma once
#include "../EngineDefines.hpp"
#include "Games/Scene/2D/ILayer.hpp"
#include "Render/Shader/Shader.hpp"

namespace ggm {

class DebugLayer final : public ILayer {
    ggm::Shader mShader;
    ggm::SparseSet<GameObject2D>* mObjects = nullptr;
public:
    DebugLayer();
    void onUpdate(float dt) override;

    void onRender(int width, int height) override;

    void onInit() override;

    void onAttach(ggm::SparseSet<GameObject2D>& data) override;
};

}
