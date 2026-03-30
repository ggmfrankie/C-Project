//
// Created by ertls on 13.03.2026.
//

#include "LayerStack.hpp"

namespace Engine {
    LayerStack::LayerStack(const Game::LayerEngineContext &ec):
        mEngineContext(ec)
    {
        mLayers.reserve(16);
    }

    LayerStack::~LayerStack() = default;

    Game::IGameLayer& LayerStack::pushLayer(std::unique_ptr<Game::IGameLayer> layer) {
        layer->onAttach(mEngineContext);
        mLayers.push_back(std::move(layer));
        return *mLayers.back();
    }

    void LayerStack::popLayer(Game::IGameLayer *layer) {
        if (const auto it = std::ranges::find(mLayers, layer, &std::unique_ptr<Game::IGameLayer>::get); it != mLayers.end()) {
            (*it)->onDetach();
            mLayers.erase(it);
        }
    }

    void LayerStack::init() const {
        for (auto& layer : mLayers) {
            layer->onInit();
        }
    }

    void LayerStack::update(const float dt) const {
        for (auto& layer : mLayers) {
            if (layer->enabled) layer->onUpdate(dt);
        }
    }

    void LayerStack::render(const int width, const int height) const {
        for (auto& layer : mLayers) {
            if (layer->enabled) layer->onRender(width, height);
        }
    }
} // Engine