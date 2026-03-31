//
// Created by Stefan on 15.03.2026.
//

#pragma once
#include "LayerStack.hpp"
#include "Render/Objects/GameObject.hpp"
#include "Render/Transformation/Camera.hpp"

namespace Engine {
    class Scene {
        std::vector<Obj::GameObject> mObjects;
        Render::Camera mCamera;
        LayerStack mLayerStack;


    public:
        Scene(Render::Input& input);

        template<typename... Args>
        Obj::GameObject& pushObject(Args&&... args) {
            mObjects.emplace_back(std::forward<Args>(args)...);
            return mObjects.back();
        }

        Game::IGameLayer& pushLayer(std::unique_ptr<Game::IGameLayer> layer) {
            return mLayerStack.pushLayer(std::move(layer));
        }

        void render(int width, int height) const;
        void update(float dt);

        void init();

        Render::Camera &getCamera() { return mCamera; }
        std::vector<Obj::GameObject>& getObjects() { return mObjects; }
    };
} // Engine