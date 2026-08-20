#pragma once

#include "Render/GUI/GuiElement.h"
typedef struct Renderer Renderer;

ElementHandle TextField_new(ElementSettings elementSettings,
                            bool (*onEnterCallback)(Element *element, Renderer *renderer));
bool TextField_runTask(Element *element, Renderer *renderer);
bool TextField_onClick(Element *element, Renderer *renderer);