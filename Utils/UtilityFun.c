//
// Created by Stefan on 22.01.2026.
//

#include "UtilityFun.h"

#include <math.h>

#include "Vector.h"

Vec3f hsv_to_rgb(const float h, const float s, const float v)
{
    const float c = v * s;
    const float x = c * (1.0f - fabsf(fmodf(h / 60.0f, 2.0f) - 1.0f));
    const float m = v - c;

    float r, g, b;

    if      (h < 60)  { r = c; g = x; b = 0; }
    else if (h < 120) { r = x; g = c; b = 0; }
    else if (h < 180) { r = 0; g = c; b = x; }
    else if (h < 240) { r = 0; g = x; b = c; }
    else if (h < 300) { r = x; g = 0; b = c; }
    else              { r = c; g = 0; b = x; }

    return (Vec3f){ r + m, g + m, b + m };
}
