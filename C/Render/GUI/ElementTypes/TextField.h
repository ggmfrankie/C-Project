#pragma once

#include "Render/GUI/GuiElement.h"
typedef struct Renderer Renderer;

typedef struct {
    int byteIndex;
    float posX;

    float blinkTimer;
    bool cursorVisible;

    size_t selectStart;
    size_t selectEnd;
} TextFieldCursor;

typedef struct {
    String text;

    TextFieldCursor cursor;

    bool focused;
    bool selecting;

    bool (*onEnterCallback)(Element* element, Renderer *renderer);
} TextFieldData;

ElementHandle TextField_new(ElementSettings elementSettings,
                            bool (*onEnterCallback)(Element *element, Renderer *renderer));
bool TextField_runTask(Element *element, Renderer *renderer);
bool TextField_onClick(Element *element, Renderer *renderer);