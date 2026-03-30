//
// Created by ertls on 13.03.2026.
//

#pragma once
#include <vector>

#include "Scene.hpp"
#include "Games/IGame.hpp"
#include "Games/IGameLayer.hpp"


namespace Engine {



    class LayerStack {

        std::vector<std::unique_ptr<Game::IGameLayer>> mLayers;
        Game::LayerEngineContext mEngineContext;

    public:
        explicit LayerStack(const Game::LayerEngineContext& ec);
        ~LayerStack();

        Game::IGameLayer& pushLayer(std::unique_ptr<Game::IGameLayer> layer);
        void popLayer(Game::IGameLayer* layer);

        void init() const;

        void update(float dt) const;
        void render(int width, int height) const;
    };
} // Engine