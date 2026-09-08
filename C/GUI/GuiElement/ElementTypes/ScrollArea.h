//
// Created by Stefan on 06.09.2026.
//

#pragma once
#include "GUI/GuiElement/GuiElement.h"

typedef struct {
    Vec2f pos;
    float width;
    float height;
    Vec3f backgroundColor;
    Vec3f sliderColor;
    Vec3f railColor;
    float childGap;

    Padding padding;

    bool notSelectable;
} ScrollAreaSettings;

ElementHandle _ScrollArea_new(ScrollAreaSettings settings, ...);

#define ScrollArea_new(...) _ScrollArea_new(__VA_ARGS__, (ElementHandle){-1})
