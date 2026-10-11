//
// Created by ertls on 17.02.2026.
//
#pragma once

#include "Utils/Math/Matrix.hpp"
#include "Utils/Math/Vector.hpp"

namespace ggm {
    class Camera {
    public:
        Camera() = default;
        ~Camera() = default;

        void lookAt();

        void moveBy(float offsetX, float offsetY, float offsetZ);

        void rotateBy(float dPitch, float dRoll, float dYaw);

        [[nodiscard]] float getFOV() const { return mFOV;}
        [[nodiscard]] float* getFOVptr() { return &mFOV;}

        void setFOV(const float FOV) { mFOV = FOV; }
        void setPos(const Vector3f v){ mPos = v; }

        [[nodiscard]] Matrix4f getViewMatrix() const;

    private:
        Vector3f mPos{};
        Vector3f mRotation{};

        float mFOV = 90.0f;
    };
}