//
// Created by ertls on 17.02.2026.
//

#ifndef MIXEDPROJECT_CAMERA_H
#define MIXEDPROJECT_CAMERA_H
#include "../../Utils/Math/Matrix.h"
#include "../../Utils/Math/Vector.h"

namespace Render {
    class Camera {
    public:
        Camera() = default;
        ~Camera() = default;

        void lookAt();

        void moveBy(float offsetX, float offsetY, float offsetZ);

        void rotateBy(float dPitch, float dRoll, float dYaw);

        [[nodiscard]] Math::Matrix4f getViewMatrix() const;

        //void movePosition(float x, float y, float z);

    private:
        Math::Vector3f m_pos{};
        Math::Vector3f m_rotation{};
    };
} // Render

#endif //MIXEDPROJECT_CAMERA_H