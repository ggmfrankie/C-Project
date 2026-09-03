#pragma once
#include "GuiTypes.h"
#include "GUI/GuiElement/GuiElement.h"
#include "Utils/Math/Vector.h"

typedef struct {
    float normPos;
    float absPos;
    void(*onMove)(float normPos, float absPos);
} ScrollbarData;

typedef struct {
    Vec2f pos;
    float railWidth;
    float sliderHeight;
    void(*onMove)(float normPos, float absPos);
} ScrollbarSettings;

ElementHandle Scrollbar_new(ScrollbarSettings settings);
