//
// Created by Stefan on 28.10.2025.
//

#ifndef C_GUIELEMENT_H
#define C_GUIELEMENT_H
#include <stdbool.h>

#include "Mesh.h"
#include "Texture.h"
#include "../Utils/SimpleArray.h"
#include "../Utils/Vector.h"

typedef enum {
    S_default = 0,
    S_active = 1,
} ElementState;

typedef struct Element {
    Vec2f pos;
    float width;
    float height;
    short meshCount;
    ElementState state;
    bool isVisible;
    Mesh* Mesh;
    Texture* texture;
    bool (*isMouseOver)(struct Element* element, Vec2f mousePos);
    void (*onClick)(struct Element* element);
    void (*onHover)(struct Element* element);
} Element;

SIMPLEARRAY(Element, Element)
ARRAY_LIST(Element, Element)

Element newElement(Mesh *mesh, short meshCount, Vec2f pos, int width, int height, Texture* texture);
void setOnClickCallback(Element* element, void (*onClick)(Element* element));
void setOnHoverCallback(Element* element, void (*onHover)(Element* element));
void setBoundingBox(Element* element, bool (*isMouseOver)(Element* element, Vec2f mousePos));
bool isSelected_Quad(Element *element, Vec2f mousePos);
#endif //C_GUIELEMENT_H