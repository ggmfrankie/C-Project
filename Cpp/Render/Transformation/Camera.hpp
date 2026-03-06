//
// Created by ertls on 17.02.2026.
//

#ifndef MIXEDPROJECT_CAMERA_H
#define MIXEDPROJECT_CAMERA_H
#include "../../Utils/Math/Matrix.hpp"
#include "../../Utils/Math/Vector.hpp"

namespace Render {
    class Camera {
    public:
        Camera() = default;
        ~Camera() = default;

        void lookAt();

        void moveBy(float offsetX, float offsetY, float offsetZ);

        void rotateBy(float dPitch, float dRoll, float dYaw);

        [[nodiscard]] float getFOV() const { return m_FOV;}
        [[nodiscard]] float* getFOVptr() { return &m_FOV;}

        void setFOV(const float FOV) { m_FOV = FOV; }
        void setPos(const Math::Vector3f v){ m_pos = v; }

        [[nodiscard]] Math::Matrix4f getViewMatrix() const;

        //void movePosition(float x, float y, float z);

    private:
        Math::Vector3f m_pos{};
        Math::Vector3f m_rotation{};

        float m_FOV = 90.0f;
    };
} // Render

#endif //MIXEDPROJECT_CAMERA_H