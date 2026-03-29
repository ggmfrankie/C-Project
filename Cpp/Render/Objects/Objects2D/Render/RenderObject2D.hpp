//
// Created by Stefan on 27.03.2026.
//

#pragma once

#include "DynamicMesh.hpp"
#include "Utils/Math/ggmdef.hpp"
#include "Utils/Math/Quaternion.hpp"
#include "Utils/Math/Vector.hpp"

namespace Obj {
    class RenderObject2D {
        ggm::i64 uuid{};
        ggm::Vector2f mPos{};

        ggm::Quaternion mRot{};
        DynamicMesh mMesh{};

    public:
        RenderObject2D();
        ~RenderObject2D();

        void init();
        void render();

        DynamicMesh& getMesh();
    };
} // Obj