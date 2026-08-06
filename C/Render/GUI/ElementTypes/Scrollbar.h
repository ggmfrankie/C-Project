#pragma once
#include "GuiTypes.h"
#include <Vector.h>

typedef struct {
    struct {
        float normPos;
        int absPos;
    } slider;
    
    int length;
} ScrollbarData;



void handleSliderDragging(Element* element, Vec2i pos);
