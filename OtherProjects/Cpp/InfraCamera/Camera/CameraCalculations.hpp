//
// Created by ertls on 16.09.2026.
//

#pragma once

#include <cmath>

struct PixelSize
{
    double width;
    double height;
};

inline double degToRad(double degrees) {
    return degrees * (M_PI / 180.0);
}

inline PixelSize CalculateObjectPixelSize(
    const double objectWidth,
    const double objectHeight,
    const double distance,
    const double horizontalFovRadians = degToRad(48),
    const double verticalFovRadians = degToRad(37),
    const double imageWidthPixels = 80.0,
    const double imageHeightPixels = 60.0)
{
    const double sceneWidthAtDistance = 2.0 * distance * std::tan(horizontalFovRadians / 2.0);
    const double sceneHeightAtDistance = 2.0 * distance * std::tan(verticalFovRadians / 2.0);

    return {
        (objectWidth / sceneWidthAtDistance) * imageWidthPixels,
        (objectHeight / sceneHeightAtDistance) * imageHeightPixels
    };
}
