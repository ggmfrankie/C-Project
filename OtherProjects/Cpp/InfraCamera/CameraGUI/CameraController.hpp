//
// Created by ertls on 14.09.2026.
//

#pragma once
#include "../Camera/Camera.hpp"

static constexpr const char* CAMERA_IP = "169.254.4.38";

class CameraController {
public:
    static Camera& Get() {
        static Camera camera {CAMERA_IP};
        return camera;
    }
};

