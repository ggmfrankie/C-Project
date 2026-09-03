#include "TextField.h"

#include <string.h>

#include "../GuiElement.h"
#include "GUI/Engine.h"
#include "../../Drawing/Mesh/Mesh.h"
#include "Utils/DataStructures/CArrayList.h"
#define CURSOR_WIDTH 4
#define CURSOR_BLINK_TIMER 1.0

static void TextField_drawCursor(const Element* element, GuiVertex** aVertices, int** aIndices, MeshInstanceData** additional, ssize_t ownerId) {
    if (element->state != UI_STATE_SELECTED) return;
    const TextFieldData* data = element->elementData.ptr;
    if (!data->cursor.isVisible) return;

    const Vec2f pos = {data->cursor.pos.abs, element->dims.pos.y};
    const Vec2f dims = {CURSOR_WIDTH, element->dims.worldHeight};

    Mesh_customQuad(
        pos,
        dims,
        (Vec4f){0.1,0.1,.1, .6},
        aVertices,
        aIndices,
        additional,
        ownerId
    );
}

static void TextField_updateBlinkTimer(Element* self, double deltaTime) {
    TextFieldData* data = self->elementData.ptr;
    data->cursor.blinkTimer += deltaTime;

    if (data->cursor.blinkTimer > 0.5 * CURSOR_BLINK_TIMER) {
        data->cursor.isVisible = false;
        if (data->cursor.blinkTimer > CURSOR_BLINK_TIMER) data->cursor.blinkTimer = 0.0;
    }
    else {
        data->cursor.isVisible = true;
    }
}

void TextField_moveCursorTo(const Element* self, int index) {
    const Character* aCharQuads = self->textElement.aCharQuads;
    TextFieldData* data = self->elementData.ptr;
    TextFieldCursor* cursor = &data->cursor;

    const int len = arrLen(aCharQuads);
    const float offset = self->padding.left - CURSOR_WIDTH;

    if (index <= 0) {
        cursor->pos = (TextFieldCursorPos){0, offset};
    }
    else if (index >= len) {
        const Character* last = arrGetLast(aCharQuads);
        cursor->pos = (TextFieldCursorPos){len ,offset + last->pos.x + last->advance};
    }
    else {
        cursor->pos = (TextFieldCursorPos){index, offset + aCharQuads[index-1].pos.x + (aCharQuads[index-1].advance)};
    }
    cursor->blinkTimer = 0;
}

void TextField_popChar(Element* self) {
    TextFieldData* data = self->elementData.ptr;
    if (data->cursor.pos.index == 0) return;

    const int index = data->cursor.pos.index-1;
    str_popCharAt(&data->text, index);
    Element_setText_ptr(self, data->text.m);

    TextField_moveCursorTo(self, index);
}

void TextField_moveCursorBy(const Element* self, int amount) {
    TextField_moveCursorTo(self, ((TextFieldData*)self->elementData.ptr)->cursor.pos.index + amount);
}

void TextField_insertCharAtCursor(Element* self, char c) {
    TextFieldData* tfd = self->elementData.ptr;

    str_appendCharAt(&tfd->text, c, tfd->cursor.pos.index);
    Element_setText_ptr(self, tfd->text.m);
    TextField_moveCursorBy(self, +1);
}

static void TextField_setClosestCursorPos(const Element* self, float pos) {
    const Character* aCharQuads = self->textElement.aCharQuads;
    float prev = 0.0;
    int index = arrLen(aCharQuads);

    for_eachArr(character, aCharQuads, {
        const float next = self->padding.left + character->pos.x + character->width * 0.5;
        if (prev <= pos && pos < next) {
            index = i;
            break;
        }
        prev = next;
    });

    TextField_moveCursorTo(self, index);
}

static bool TextField_onClick(Element *self) {
    if(self->type != t_textField) return false;

    const TextFieldData* data = self->elementData.ptr;
    if (Strings.isEmpty(&data->text)) return false;

    const Vec2f mousePos = getMousePos();

    const float relMouseX = mousePos.x - self->dims.worldPos.x;

    TextField_setClosestCursorPos(self, relMouseX);
    return true;
}

ElementHandle TextField_new(const ElementSettings elementSettings, bool (*onEnterCallback)(Element *element)) {
    const ElementHandle element = createElement(elementSettings);
    TextFieldData* textData = calloc(1, sizeof(TextFieldData));
    textData->onEnterCallback = onEnterCallback;
    const ElementHandle textField = createElement(
        (ElementSettings){
            .minWidth = elementSettings.minWidth,
            .minHeight = elementSettings.minHeight,
            .padding = {5,5,5,5},
            .elementData = textData,
            .color = v_mul(elementSettings.color, 0.8f),
            .onClick = TextField_onClick,
            .text = "",
            .task = elementSettings.task,
            .drawCustom = TextField_drawCursor,
            .whileSelected = TextField_updateBlinkTimer
        }
    );
    Element_get(textField)->type = t_textField;
    Element_get(element)->type = t_textField;
    addChildElements(Element_get(element), textField);

    return element;
}

bool TextField_runTask(Element *element) {
    if(element->type != t_textField) return false;
    TextFieldData* data = element->elementData.ptr;
    if (data->text.length == 0) return false;

    char* newBuffer = malloc(data->text.length + 1);
    memcpy(newBuffer, data->text.m, data->text.length);
    newBuffer[data->text.length] = '\0';

    str_clear(&data->text);
    Element_setText_ptr(element,"");
    TextField_moveCursorTo(element, 0);

    if (element->task.func && !element->task.isBlocked) {
        pushTask(element->task.func, newBuffer);
    }
    return true;
}
