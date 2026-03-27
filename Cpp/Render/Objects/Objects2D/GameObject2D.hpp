//
// Created by Stefan on 27.03.2026.
//

#pragma once

#include "RenderObject2D.hpp"
#include "Engine/Core/PhysixBox/SoftBody2D.hpp"

namespace Obj {
    class GameObject2D {
        RenderObject2D* mRenderObj = nullptr;
        PhysixBox::SoftBody2D* mPhysicsObj = nullptr;

    };
} // Obj