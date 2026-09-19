//
// Created by Stefan on 06.05.2026.
//

#pragma once
#include "../EngineDefines.hpp"
#include <memory>

#include "ILayer.hpp"
#include "Utils/DataStorage/InlineVector.hpp"

namespace Game2D {

class LayerStack {
    ggm::SparseSet<GameObject>& mObjects;
    ggm::InlineVector<std::unique_ptr<ILayer>, 4> mLayer;
public:
    explicit LayerStack(ggm::SparseSet<GameObject>& objects);
    void pushLayer(std::unique_ptr<ILayer> layer);

    void updateAll(float dt);
    void renderAll(int width, int height);
    void initAll();
};

}
