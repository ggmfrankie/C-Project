//
// Created by Stefan on 06.05.2026.
//

#pragma once
#include "EngineDefines.hpp"
#include "Render/Objects/2D/GameObject.hpp"
#include "Utils/DataStorage/SparseSet.hpp"

namespace Game2D {

class ILayer {
public:
    virtual ~ILayer() = default;
    virtual void onUpdate(float dt) = 0;
    virtual void onRender(int width, int height) = 0;
    virtual void onAttach(ggm::SparseSet<GameObject>& data) = 0;
    virtual void onInit() = 0;

    bool enabled = true;
};

} // Scene2d
