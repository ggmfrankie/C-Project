//
// Created by ertls on 17.02.2026.
//

#include "Camera.hpp"

#include "../../Utils/StringUtils.hpp"

namespace ggm {
    void Camera::lookAt() {

    }

    void Camera::moveBy(const float offsetX, const float offsetY, const float offsetZ) {

        const float yawRad = ggm::toRad(mRotation.z);

        // Forward (XZ) consistent with view R = Ry(-yaw) * Rx(-pitch): forward is -Z at yaw=0
        const float fx =  std::sin(yawRad);
        const float fz = -std::cos(yawRad);

        // Right (XZ)
        const float rx =  std::cos(yawRad);
        const float rz =  std::sin(yawRad);

        // Apply movement in camera-aligned horizontal axes
        mPos.x += rx * offsetX + fx * offsetZ;
        mPos.z += rz * offsetX + fz * offsetZ;
        mPos.y += offsetY;

    }

    void Camera::rotateBy(const float dPitch, const float dRoll, const float dYaw) {
        mRotation.x += dPitch;
        mRotation.y += dRoll;
        mRotation.z += dYaw;
    }

    ggm::Matrix4f Camera::getViewMatrix() const {
        using namespace ggm;

        const float pitchDeg = mRotation.x;
        const float yawDeg   = mRotation.z;

        // View is inverse rotation, hence the negatives:
        const Matrix4f R = Matrix4f::RotationX(-pitchDeg) * Matrix4f::RotationY(-yawDeg);
        return R * Matrix4f::Translation(-mPos);
    }
} // Render