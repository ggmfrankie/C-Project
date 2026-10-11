//
// Created by Stefan on 27.03.2026.
//

#pragma once
#include "../EngineDefines.hpp"
#include "DynamicMesh2D.hpp"
#include "../../../../Utils/ggmdef.hpp"
#include "Utils/Math/Quaternion.hpp"
#include "Utils/Math/Vector.hpp"

namespace ggm {
    class RenderObject2D {
        i64 uuid{};
        Vector2f mPos{};

        Quaternion mRot{};
        DynamicMesh2D mMesh;
        Vector3f mColor{};

    public:
        RenderObject2D(const Vector3f& color, const std::vector<Vector2f> &pos);
        ~RenderObject2D();

        void init();
        void render() const;

        [[nodiscard]] Vector3f getColor() const;
        DynamicMesh2D& getMesh();
    };
} // Obj