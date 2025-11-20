//
// Created by Stefan on 28.10.2025.
//

#include "../GUI/GuiElement.h"

#include "../Render.h"

Element newElement(Mesh *mesh, const short meshCount, const Vec2f pos, const int width, const int height, Texture* texture) {
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
        .userdata = NULL
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

void setOnHoverCallback(Element* element, void (*onHover)(Element* element, Renderer* renderer)) {
    element->onHover = onHover;
}

void setOnClickCallback(Element* element, void (*onClick)(Element* element, Renderer* renderer)) {
    element->onClick = onClick;
}

