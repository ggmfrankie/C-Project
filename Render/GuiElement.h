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
struct Element;


typedef struct Element {
    Vec2f pos;
    float width;
    float height;
    short meshCount;
    bool isVisible;
    Mesh* Mesh;
    Texture* texture;
    bool (*calcHitbox)(struct Element* element);
    void (*onClick)(struct Element* element);
    void (*onHover)(struct Element* element);
} Element;

SIMPLEARRAY(Element, Element)
ARRAY_LIST(Element, Element)

Element newElement(Mesh *mesh, short meshCount, Vec2f pos, int width, int height, Texture* texture);
#endif //C_GUIELEMENT_H