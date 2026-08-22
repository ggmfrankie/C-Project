//
// Created by Stefan on 12.11.2025.
//

#pragma once
#include "GuiDefines.h"
#include "GuiTypes.h"
#include "Utils/Math/Vector.h"

void onRequestMove_SimpleDrag(Element* element, Vec2f pos);

bool runTaskFun(Element *element, Renderer *renderer);
bool onHover_changeColor(Element *element, Renderer *renderer);

void updateColorRainbow(Element *element);
void displayCurrentTime(Element *element);
