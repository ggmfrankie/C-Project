//
// Created by ertls on 17.02.2026.
//

#include "Camera.h"

#include "../../Utils/Utils.h"

namespace Render {
    void Camera::moveRotation(const float dx, const float dy, const float dz) {
        m_rotation.x += dx;
        m_rotation.y += dy;
        m_rotation.z += dz;
    }

    void Camera::lookAt() {

    }

    void Camera::moveBy(const float offsetX, const float offsetY, const float offsetZ) {
        if ( offsetZ != 0 ) {
            m_pos.x += std::sin(Math::Matrix4f::toRad(m_rotation.y)) * -1.0f * offsetZ;
            m_pos.z += std::cos(Math::Matrix4f::toRad(m_rotation.y)) * offsetZ;
        }
        if ( offsetX != 0) {
            m_pos.x += std::sin(Math::Matrix4f::toRad(m_rotation.y - 90)) * -1.0f * offsetX;
            m_pos.z += std::cos(Math::Matrix4f::toRad(m_rotation.y - 90)) * offsetX;
        }
        m_pos.y += offsetY;
    }

    void Camera::rotateBy(const float dPitch, const float dRoll, const float dYaw) {
        m_rotation.x += dPitch;
        m_rotation.y += dRoll;
        m_rotation.z += dYaw;
    }

    Math::Matrix4f Camera::getViewMatrix() const {
        using namespace Math;
        return Matrix4f::Rotation(-m_rotation) * Matrix4f::Translation(-m_pos);
    }


} // Render