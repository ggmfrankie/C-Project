//
// Created by Stefan on 06.05.2026.
//

#include "LayerStack.hpp"

namespace Game2D {
    LayerStack::LayerStack(ggm::SparseSet<GameObject>& objects) :
        mObjects(objects)
    {}

    void LayerStack::pushLayer(std::unique_ptr<ILayer> layer) {
        layer->onAttach(mObjects);
        mLayer.add(std::move(layer));
    }

    void LayerStack::updateAll(float dt) {
        for (const auto& layer: mLayer) {
            if (layer->enabled) layer->onUpdate(dt);
        }
    }

    void LayerStack::renderAll(int width, int height) {
        for (const auto& layer: mLayer) {
            if (layer->enabled) layer->onRender(width, height);
        }
    }

    void LayerStack::initAll() {
        for (const auto& layer: mLayer) {
            if (layer->enabled) layer->onInit();
        }
    }
}
