#pragma once
#include "GuiTypes.h"
#include "GUI/GuiElement/GuiElement.h"
#include "Utils/Math/Vector.h"

typedef struct {
    ElementHandle slider;
    float progress;
    void(*onMove)(float normPos, float absPos, Element* moveElement);
    ElementHandle moveElement;
} ScrollbarData;

typedef struct {
    Vec2f pos;
    float railWidth;
    float sliderHeight;
    void(*onMove)(float normPos, float absPos, Element* moveElement);
    ElementHandle moveElement;
} ScrollbarSettings;

ElementHandle Scrollbar_new(ScrollbarSettings settings);
void Scrollbar_updatePos(const Element* rail);
void Scrollbar_setSliderHeight(const Element* rail, float newHeight);
