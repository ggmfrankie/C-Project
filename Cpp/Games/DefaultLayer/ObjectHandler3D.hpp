//
// Created by Stefan on 31.03.2026.
//

#pragma once
#include "Games/IGameLayer.hpp"

namespace Game {
    class ObjectHandler3D final : public IGameLayer{
    public:
        ~ObjectHandler3D() override;

        void onAttach(const LayerEngineContext &ec) override;

        void onInit() override;

        void onDetach() override;

        void onUpdate(float dt) override;

        void onRender(int width, int height) override;
    };
} // Game