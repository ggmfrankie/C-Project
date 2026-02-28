//
// Created by ertls on 17.02.2026.
//

#include "Camera.hpp"

#include "../../Utils/Utils.hpp"

namespace Render {
    void Camera::lookAt() {

    }

    void Camera::moveBy(const float offsetX, const float offsetY, const float offsetZ) {

        const float yawRad = Math::Matrix4f::toRad(m_rotation.z);

        // Forward (XZ) consistent with view R = Ry(-yaw) * Rx(-pitch): forward is -Z at yaw=0
        const float fx =  std::sin(yawRad);
        const float fz = -std::cos(yawRad);

        // Right (XZ)
        const float rx =  std::cos(yawRad);
        const float rz =  std::sin(yawRad);

        // Apply movement in camera-aligned horizontal axes
        m_pos.x += rx * offsetX + fx * offsetZ; // strafe + forward
        m_pos.z += rz * offsetX + fz * offsetZ;
        m_pos.y += offsetY;

    }

    void Camera::rotateBy(const float dPitch, const float dRoll, const float dYaw) {
        m_rotation.x += dPitch;
        m_rotation.y += dRoll;
        m_rotation.z += dYaw;
    }

    Math::Matrix4f Camera::getViewMatrix() const {
        using namespace Math;

        const float pitchDeg = m_rotation.x;
        const float yawDeg   = m_rotation.z;

        // View is inverse rotation, hence the negatives:
        const Matrix4f R = Matrix4f::RotationX(-pitchDeg) * Matrix4f::RotationY(-yawDeg);
        return R * Matrix4f::Translation(-m_pos);

    }
} // Render