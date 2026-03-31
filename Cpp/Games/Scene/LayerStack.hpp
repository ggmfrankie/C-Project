//
// Created by ertls on 13.03.2026.
//

#pragma once
#include <vector>

#include "Games/IGameLayer.hpp"
#include <memory>


namespace Engine {

    class LayerStack {

        std::vector<std::unique_ptr<Game::IGameLayer>> mLayers;
        Game::LayerEngineContext mEngineContext;

    public:
        explicit LayerStack();
        ~LayerStack();

        Game::IGameLayer& pushLayer(std::unique_ptr<Game::IGameLayer> layer);
        void popLayer(Game::IGameLayer* layer);

        void onAttach(const Game::LayerEngineContext& ec);
        void init() const;

        void update(float dt) const;
        void render(int width, int height) const;
    };
} // Engine