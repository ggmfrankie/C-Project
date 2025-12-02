//
// Created by Stefan on 28.10.2025.
//

#include "../GUI/GuiElement.h"

#include <pthread.h>
#include <stdarg.h>

#include "../Render.h"
#include "../Engine.h"

Element newElement(const Mesh mesh, const short meshCount, const Vec2f pos, const int width, const int height, Texture* texture) {
    puts("new Element created");
    return (Element){
        .Mesh = mesh,
        .meshCount = meshCount,
        .state = 0,
        .isVisible = true,
        .onClick = NULL,
        .onHover = NULL,
        .isMouseOver = NULL,
        .pos = pos,
        .worldPos = pos,
        .width = (float) width,
        .height = (float) height,
        .actualWidth = (float) width,
        .actualHeight = (float) height,
        .texture = texture,
        .color = (Vec3f){0.0f, 0.0f, 0.0f},
        .textElement = NULL,
        .parentElement = NULL,
        .childElements = ChildElements_newList(8),
        .padding = (Padding){0,0,0,0},
        .hasText = false,
        .task = (Task){NULL, NULL},
        .autoFit = true
    };
}

bool isSelected_Quad(const Element *element, const Vec2f mousePos) {
    if (mousePos.x <= element->worldPos.x+element->actualWidth && mousePos.x >= element->worldPos.x &&
        mousePos.y <= element->worldPos.y+element->actualHeight && mousePos.y >= element->worldPos.y) {
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

Element* f_addChildElements(Element* parent, ...)
{va_list args;
    va_start(args, parent);

    while (1) {
        Element* child = va_arg(args, Element*);
        if (child == NULL) {
            // Encountered the terminating NULL (or a NULL child). Log and stop.
            printf("f_addChildElements: encountered NULL child for parent=%p\n", (void*)parent);
            break;
        }

        // Defensive: sanity-check child pointer range (heuristic)
        uintptr_t p = (uintptr_t)child;
        if (p < 0x1000) {
            printf("f_addChildElements: suspicious child pointer %p for parent=%p\n", (void*)child, (void*)parent);
            break;
        }

        child->parentElement = parent;
        ChildElements_ListAdd(&parent->childElements, child);
    }

    va_end(args);
    return parent;
}

Element* f_addChildElementsN(Element* parent, const int count, ...)
{
    va_list args;
    va_start(args, count);
    for (int i = 0; i < count; i++) {
        Element* child = va_arg(args, Element*);
        if (!child) continue; // skip NULLs
        child->parentElement = parent;
        ChildElements_ListAdd(&parent->childElements, child);
    }
    va_end(args);
    return parent;
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

