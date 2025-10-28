//
// Created by Stefan on 28.10.2025.
//

#ifndef C_GUIELEMENT_H
#define C_GUIELEMENT_H
#include <stdbool.h>

#include "Mesh.h"
#include "../Utils/SimpleArray.h"



typedef struct Element {
    float x;
    float y;
    short meshCount;
    bool isVisible;
    Mesh* Mesh;
    void (*onClick)(struct Element* element);
    void (*onHover)(struct Element* element);
} Element;

SIMPLEARRAY(Element, Element)

#endif //C_GUIELEMENT_H