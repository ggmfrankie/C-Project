//
// Created by ertls on 14.09.2026.
//

#include "CameraController.hpp"

#include <iostream>

#include "GuiInterface.h"

extern "C" void pollData(void*) {
    auto toDeg = [](float f){return f - 273.15;};
    try {
        auto& cam = CameraController::Get();
        auto [internalTemp, spot1, box1Min, box1Max, box1Avg, spot2, spot3] = cam.pollData<Camera::ThermalData>();
        gui_setTextF("Spot1", "Spot 1: %.1f", toDeg(spot1));
        gui_setTextF("Spot2", "Spot 2: %.1f", toDeg(spot2));
        gui_setTextF("Spot3", "Spot 3: %.1f", toDeg(spot3));

        gui_setTextF("Box1 Min", "Box1 Min: %.1f", toDeg(box1Min));
        gui_setTextF("Box1 Max", "Box1 Max: %.1f", toDeg(box1Max));
        gui_setTextF("Box1 Avg", "Box1 Avg: %.1f", toDeg(box1Avg));
    } catch (std::system_error& e) {
        std::cerr << e.what() << std::endl;
    }

}
