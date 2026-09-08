#pragma once
#include "GuiTypes.h"
#include "GUI/GuiElement/GuiElement.h"
#include "Utils/Math/Vector.h"

typedef struct ScrollbarData {
    ElementHandle slider;
    ElementHandle rail;
    float progress;
    void(*onMove)(const struct ScrollbarData* scrollbar, Element* moveElement);
    ElementHandle moveElement;
} ScrollbarData;

typedef struct {
    Vec3f sliderColor;
    Vec3f railColor;
    Vec2f pos;
    float railWidth;
    float sliderHeight;
    void(*onMove)(const ScrollbarData* scrollbar, Element* moveElement);
    ElementHandle moveElement;
} ScrollbarSettings;

ElementHandle Scrollbar_new(ScrollbarSettings settings);
void Scrollbar_updatePos(const Element* rail);
void Scrollbar_setSliderHeight(const Element* rail, float newHeight);
