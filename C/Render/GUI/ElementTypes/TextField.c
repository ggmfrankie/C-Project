#include "TextField.h"

#include <string.h>

#include "../GuiElement.h"
#include "Render/Engine.h"
#include "Render/Drawing/Mesh.h"
#include "Utils/DataStructures/CArrayList.h"

static void TextField_drawCursor(Element* element, GuiVertex** aVertices, int** aIndices, MeshInstanceData** additional, ssize_t ownerId) {
    const TextFieldData* data = element->elementData.ptr;

    const Vec2f pos = {data->cursor.pos.abs, element->dims.pos.y};
    const Vec2f dims = {2, element->dims.worldHeight};

    Mesh_customQuad(
        pos,
        dims,
        (Vec4f){0,0.7,.6, 1.0},
        aVertices,
        aIndices,
        additional,
        ownerId
    );
}

static TextFieldCursorPos TextField_getClosestCursorPos(const Character* aCharQuads, float pos, float padding) {
    float prev = 0.0;
    printf("X pos = %f\n", pos);
    for_eachArr(character, aCharQuads, {
        const float next = padding + character->pos.x + (character->width) * 0.5;
        if (prev <= pos && pos < next) {
            printf("char is %c\n", character->value);
            if (i == 0) { return (TextFieldCursorPos){0, padding};}
            else { return (TextFieldCursorPos){i, padding + aCharQuads[i-1].pos.x + (aCharQuads[i-1].width)};}
        }
        prev = next;
    });
    const Character* last = arrGetLast(aCharQuads);
    return (TextFieldCursorPos){arrLen(aCharQuads) ,padding + last->pos.x + last->width};
}

static TextFieldCursorPos TextField_moveCursorBy(){}

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
            .drawCustom = TextField_drawCursor
        }
    );
    Element_get(textField)->type = t_textField;
    Element_get(element)->type = t_textField;
    addChildElements(Element_get(element), textField);

    return element;
}

bool TextField_onClick(Element *element) {
    if(element->type != t_textField) return false;

    TextFieldData* data = element->elementData.ptr;
    if (Strings.isEmpty(&data->text)) return false;

    const Character* aCharQuads = element->textElement.aCharQuads;
    const Vec2f mousePos = getMousePos();

    const float relMouseX = mousePos.x - element->dims.worldPos.x;

    data->cursor.pos = TextField_getClosestCursorPos(aCharQuads, relMouseX, element->padding.left);
    return true;
}

bool TextField_runTask(Element *element) {
    if(element->type != t_textField) return false;
    TextFieldData* data = element->elementData.ptr;
    if (data->text.length == 0) return false;

    char* newBuffer = malloc(data->text.length + 1);
    memcpy(newBuffer, data->text.m, data->text.length);
    newBuffer[data->text.length] = '\0';

    str_clear(&data->text);
    data->cursor.pos.index = 0;
    Element_setText_ptr(element,"");

    if (element->task.func && !element->task.isBlocked) {
        pushTask(element->task.func, newBuffer);
    }
    return true;
}
