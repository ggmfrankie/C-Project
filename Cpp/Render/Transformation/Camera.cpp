//
// Created by ertls on 17.02.2026.
//

#include "Camera.h"

namespace Render {
    Camera::Camera() : viewMatrix(), pos(0, 0, -3), rotation(0, 0, 0) {
    }

    void Camera::moveRotation(const float dx, const float dy, const float dz) {
        rotation.x += dx;
        rotation.y += dy;
        rotation.z += dz;
    }

    void Camera::lookAt() {

    }

} // Render