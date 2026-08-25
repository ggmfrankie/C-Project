#pragma once
#include "GuiTypes.h"
#include "Render/GUI/GuiElement.h"
#include "Utils/Math/Vector.h"

typedef struct {
    struct {
        float normPos;
        float absPos;
    } slider;
    
    float length;
} ScrollbarData;

typedef struct {
    Vec2f pos;
    float railWidth;
    float sliderWidth;
} ScrollbarSettings;

ElementHandle Scrollbar_new(ScrollbarSettings settings);
void Scrollbar_handleDragging(Element* element, Vec2f pos);
