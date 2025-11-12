//
// Created by Stefan on 12.11.2025.
//

#include "CallbackFunctions.h"
inline void callback1(Element *element) {
    element->state = 1;
}

void dragFunction(Element *element) {
    static Vec2f initialMousePos;
    static bool wasDragged = false;

    if (!wasDragged) {

    }
}