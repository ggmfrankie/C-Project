//
// Created by Stefan on 27.03.2026.
//

#pragma once

#include "DynamicMesh.hpp"
#include "Utils/Math/ggmdef.hpp"
#include "Utils/Math/Quaternion.hpp"
#include "Utils/Math/Vector.hpp"

namespace Game2D {
    class RenderObject {
        ggm::i64 uuid{};
        ggm::Vector2f mPos{};

        ggm::Quaternion mRot{};
        DynamicMesh mMesh;
        ggm::Vector3f mColor{};

    public:
        RenderObject(const ggm::Vector3f& color, const std::vector<ggm::Vector2f> &pos);
        ~RenderObject();

        void init();
        void render() const;

        [[nodiscard]] ggm::Vector3f getColor() const;
        DynamicMesh& getMesh();
    };
} // Obj