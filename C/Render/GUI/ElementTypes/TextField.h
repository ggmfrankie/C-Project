#pragma once

#include "Render/GUI/GuiElement.h"

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

    bool (*onEnterCallback)(Element* element);
} TextFieldData;

ElementHandle TextField_new(ElementSettings elementSettings,
                            bool (*onEnterCallback)(Element *element));
bool TextField_runTask(Element *element);
bool TextField_onClick(Element *element);