//
// Created by ertls on 13.03.2026.
//
#pragma once

namespace Game{
    class IGameLayer {
        public:
        virtual ~IGameLayer() = default;

        virtual void onAttach() = 0;
        virtual void onDetach() = 0;

        virtual void onUpdate(float dt) = 0;
        virtual void onRender() = 0;
    };
}
