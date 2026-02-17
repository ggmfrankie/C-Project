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
        Camera();
        ~Camera() = default;
        void update();
        void moveRotation(float dx, float dy, float dz);
        void movePosition(float x, float y, float z);
        private:

        Math::Matrix4f viewMatrix;
        Math::Vector3f pos;
        Math::Vector3f rotation;
    };
} // Render

#endif //MIXEDPROJECT_CAMERA_H