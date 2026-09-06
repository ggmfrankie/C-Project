//
// Created by Stefan on 06.09.2026.
//

#pragma once
#include "GUI/GuiElement/GuiElement.h"

typedef struct {
    Vec2f pos;
    float width;
    float height;
} ScrollAreaSettings;

ElementHandle _ScrollArea_new(ScrollAreaSettings settings, ...);

#define ScrollArea_new(settings, ...) _ScrollArea_new(settings, __VA_ARGS__, (ElementHandle){-1})
