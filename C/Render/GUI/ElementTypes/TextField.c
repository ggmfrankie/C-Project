#include "TextField.h"

#include <string.h>

#include "../GuiElement.h"
#include "Render/Engine.h"
#include "Render/Drawing/Mesh.h"
#include "Utils/DataStructures/CArrayList.h"

static void TextField_drawCursor(Element* element, GuiVertex** aVertices, int** aIndices, MeshInstanceData** additional) {
    const TextFieldData* data = element->elementData.ptr;
    Mesh_customQuad(
        element,
        (Vec2f){data->cursor.pos.abs, element->dims.worldHeight},
        (Vec2f){5, 10},
        (Vec4f){0,0,.6, 0.3},
        aVertices,
        aIndices,
        additional
    );
}

static TextFieldCursorPos TextField_getClosestCursorPos(const Character* aCharQuads, float pos) {
    float prev = 0.0;
    for_eachArr(character, aCharQuads, {
        const float next = character->pos.x + character->width/2;
        if (prev <= pos && pos < next) {
            if (i == 0) return (TextFieldCursorPos){0, 0.0};
            return (TextFieldCursorPos){i, prev + (next - prev)*0.5};
        }
        prev = next;
    });
    const Character* last = arrGetLast(aCharQuads);
    return (TextFieldCursorPos){arrLen(aCharQuads)+1 ,last->pos.y + last->width + last->advance};
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

    Character* aCharQuads = element->textElement.aCharQuads;
    const Vec2f mousePos = getMousePos();

    const Vec2f relMousePos = {(mousePos.x - element->dims.pos.x), (mousePos.y - element->dims.pos.y)};

    data->cursor.pos = TextField_getClosestCursorPos(aCharQuads, mousePos.x);
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
