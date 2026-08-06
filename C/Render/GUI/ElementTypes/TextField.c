#include "TextField.h"

#include <string.h>

#include "../GuiElement.h"
#include "Render/Engine.h"
#include "Render/GUI/GuiElementData.h"
#include "Utils/DataStructures/CArrayList.h"

Element *TextField_new(const ElementSettings elementSettings, bool (*onEnterCallback)(Element* element, Renderer *renderer)) {
    Element* element = createElement(elementSettings);
    TextFieldData* textData = calloc(1, sizeof(TextFieldData));
    textData->onEnterCallback = onEnterCallback;
    Element* textField = createElement(
        (ElementSettings){
            .minWidth = elementSettings.minWidth,
            .minHeight = elementSettings.minHeight,
            .padding = {5,5,5,5},
            .elementData = textData,
            .color = v_mul(elementSettings.color, 0.8f),
            .onClick = TextField_onClick,
            .text = "",
            .task = elementSettings.task,
        }
    );
    textField->type = t_textField;
    element->type = t_textField;
    addChildElements(element, textField);

    return element;
}

bool TextField_onClick(Element *element, Renderer *renderer) {
    if(element->type != t_textField) return false;

    TextFieldData* data = element->elementData;
    if (Strings.isEmpty(&data->text)) return false;

    Character* charQuads = element->textElement.aCharQuads;
    Vec2i mousePos = getMousePos();

    const Vec2f relMousePos = {(float)(mousePos.x - element->dims.pos.x), (float)(mousePos.y - element->dims.pos.y)};
    mousePos.x -= element->dims.pos.x;
    mousePos.y -= element->dims.pos.y;
    int i = 0;
    for (; i < arrLen(charQuads); i++) {
        const Character* currentChar = &charQuads[i];
        if (relMousePos.x < currentChar->pos.x + currentChar->width/2) {
            data->cursor.byteIndex = i;
            return true;
        }
    }
    data->cursor.byteIndex = i;
    return true;
}

bool TextField_runTask(Element *element, Renderer *renderer) {
    if(element->type != t_textField) return false;
    TextFieldData* data = element->elementData;
    if (data->text.length == 0) return false;

    char* newBuffer = malloc(data->text.length + 1);
    memcpy(newBuffer, data->text.m, data->text.length);
    newBuffer[data->text.length] = '\0';

    str_clear(&data->text);
    data->cursor.byteIndex = 0;
    Element_setText(element,"");

    if (element->task.func && !element->task.isBlocked) {
        pushTask(element->task.func, newBuffer);
    }
    return true;
}