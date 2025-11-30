//
// Created by Stefan on 28.10.2025.
//

#include "../GUI/GuiElement.h"

#include <pthread.h>

#include "../Render.h"
#include "../Engine.h"

Element newElement(Mesh mesh, const short meshCount, const Vec2f pos, const int width, const int height, Texture* texture) {
    return (Element){
        .Mesh = mesh,
        .meshCount = meshCount,
        .state = 0,
        .isVisible = true,
        .onClick = NULL,
        .onHover = NULL,
        .isMouseOver = NULL,
        .pos = pos,
        .width = (float) width,
        .height = (float) height,
        .texture = texture,
        .textElement = NULL,
        .childElements = NULL,
        .numChildElements = 0,
        .hasText = false,
        .task = (Task){NULL, NULL}
    };
}

bool isSelected_Quad(const Element *element, const Vec2f mousePos) {
    if (mousePos.x <= element->pos.x+element->width && mousePos.x >= element->pos.x &&
        mousePos.y <= element->pos.y+element->height && mousePos.y >= element->pos.y) {
        return true;
    }
    return false;
}

void setBoundingBox(Element* element, bool (*isMouseOver)(const Element* element, Vec2f mousePos)) {
    element->isMouseOver = isMouseOver;
}

void setOnHoverCallback(Element* element, bool (*onHover)(Element* element, Renderer* renderer)) {
    element->onHover = onHover;
}

void setOnClickCallback(Element* element, bool (*onClick)(Element* element, Renderer* renderer)) {
    element->onClick = onClick;
}

void setText(Element* element, const char* text) {
    pthread_mutex_lock(&guiMutex);
    Strings.setContent_c(&element->textElement.text, text);
    element->hasText = true;
    pthread_mutex_unlock(&guiMutex);
}

void setText_int(Element* element, const int i) {
    char tempText[512];
    str_fromInt(tempText, 512, i);

    setText(element, tempText);
}

