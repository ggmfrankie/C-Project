#pragma once

#include "Render/GUI/GuiElement.h"

typedef struct {
    int index;
    float abs;
} TextFieldCursorPos;

typedef struct {
    TextFieldCursorPos pos;

    double blinkTimer;
    bool isVisible;
} TextFieldCursor;

typedef struct {
    String text;
    TextFieldCursor cursor;

    bool (*onEnterCallback)(Element* element);
} TextFieldData;

ElementHandle TextField_new(ElementSettings elementSettings,
                            bool (*onEnterCallback)(Element *element));
bool TextField_runTask(Element *element);

void TextField_moveCursorTo(const Element* self, int index);

void TextField_popChar(Element* self);
void TextField_moveCursorBy(const Element* self, int amount);

void TextField_insertCharAtCursor(Element* self, char c);