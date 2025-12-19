//
// Created by Stefan on 28.10.2025.
//

#include "../GUI/GuiElement.h"

#include <pthread.h>
#include <stdarg.h>

#include "CallbackFunctions.h"
#include "../Render.h"
#include "../Engine.h"
#include "GuiElementData.h"
#define defaultColor (Vec3f){0.5, 0.5, 0.5}

Element newElement(const Mesh mesh, const Vec2i pos, const int width, const int height, Texture* texture) {
    return (Element){
        .name = NULL,
        .Mesh = mesh,
        .state = 0,
        .onClick = NULL,
        .onHover = NULL,
        .isMouseOver = NULL,
        .pos = pos,
        .worldPos = pos,
        .width = width,
        .height = height,
        .actualWidth = width,
        .actualHeight = height,
        .texture = texture,
        .color = (Vec3f){0.0f, 0.0f, 0.0f},
        .textElement = NULL,
        .parentElement = NULL,
        .childElements = ChildElements_newList(8),
        .padding = (Padding){0,0,0,0},
        .hasText = false,
        .task = (Task){NULL, NULL},
        .autoFit = true,
        .childGap = 10,
        .ElementData = NULL,
        .needsDeletion = false,
        .isActive = true
    };
}

bool isSelected_Quad(const Element *element, const Vec2i mousePos) {
    if (mousePos.x <= element->worldPos.x+element->actualWidth && mousePos.x >= element->worldPos.x &&
        mousePos.y <= element->worldPos.y+element->actualHeight && mousePos.y >= element->worldPos.y) {
        return true;
    }
    return false;
}

void setBoundingBox(Element* element, bool (*isMouseOver)(const Element *element, Vec2i mousePos)) {
    element->isMouseOver = isMouseOver;
}

void setOnHoverCallback(Element* element, bool (*onHover)(Element* element, Renderer* renderer)) {
    element->onHover = onHover;
}

void setOnClickCallback(Element* element, bool (*onClick)(Element* element, Renderer* renderer)) {
    element->onClick = onClick;
}

Element* f_addChildElements(Element* parent, ...) {
    va_list args;
    va_start(args, parent);

    while (1) {
        Element* child = va_arg(args, Element*);
        if (child == NULL) {
            break;
        }

        child->parentElement = parent;
        ChildElements_ListAdd(&parent->childElements, child);
    }

    va_end(args);
    return parent;
}

[[deprecated]]
Element* f_addChildElementsN(Element* parent, const int count, ...) {
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

Element *guiAddElement(
    List_Element *list,
    char *name,
    const Mesh mesh,
    const Vec2i pos,
    const int width,
    const int height,
    Texture *tex,
    const Vec3f color,
    const Padding padding,
    bool (*mouseOver)(const Element *, Vec2i),
    bool (*hover)(Element *, Renderer *),
    bool (*click)(Element *, Renderer *),
    const Task task,
    const char *text,
    const bool forceResize
)
{
    Element_ListAdd(list, newElement(mesh, pos, width, height, tex));
    Element* lastElement = Element_ListGetLast(list);
    if (mouseOver) {
        lastElement->isMouseOver = mouseOver;
        if (hover) lastElement->onHover = hover;
        if (click) {
            lastElement->onClick = click;
            if (task.func) {
                lastElement->task = task;
                if (task.userdata == NULL) lastElement->task.userdata = lastElement;
            }
        }
    }
    lastElement->color = color;
    lastElement->padding = padding;
    lastElement->name = name;

    if (text) {

        lastElement->textElement = (TextElement) {
            .offset = (Vec2i){2, 2},
            .text = newReservedString(512),
            .textScale = 1.0f,
            .textColor = (Vec3f){1.0f, 1.0f, 1.0f},
            .forceResize = forceResize,
        };
        setText(lastElement, text);
        lastElement->hasText = true;
    }
    return lastElement;
}

Element *guiAddSimpleRectangle_Texture(
    List_Element *list,
    const Vec2i pos,
    const int width,
    const int height,
    Texture *tex
)
{
    Element* element = guiAddElement(list, NULL, quadMesh, pos, width, height, tex, (Vec3f){0.6f, 0.6f, 0.6f}, (Padding){10, 10, 10, 10}, NULL, NULL, NULL, (Task){NULL, NULL}, NULL,false);
    return element;
}

Element *guiAddSimpleRectangle_Color(
    List_Element *list,
    const Vec2i pos,
    const int width,
    const int height,
    const Vec3f color
)
{
    Element* element = guiAddElement(list, NULL, quadMesh, pos, width, height, NULL, color, (Padding){10, 10, 10, 10}, NULL, NULL, NULL, (Task){NULL, NULL}, NULL,false);
    return element;
}

Element *guiAddSimpleButton_Texture(
    List_Element *list,
    const Vec2i pos,
    const int width,
    const int height,
    Texture *tex,
    const Task task,
    const char *text
)
{
    Element* element = guiAddElement(list, NULL, quadMesh, pos, width, height, tex, (Vec3f){0.6f, 0.6f, 0.6f}, (Padding){10, 10, 10, 10}, isSelected_Quad, hoverCallbackFunction, clickCallbackFunction, task, text,true);
    return element;
}

Element *guiAddSimpleButton_Color(
    List_Element *list,
    const Vec2i pos,
    const int width,
    const int height,
    const Vec3f color,
    const Task task,
    const char *text
)
{
    Element* element = guiAddElement(list, NULL, quadMesh, pos, width, height, NULL, color, (Padding){10, 10, 10, 10}, isSelected_Quad, hoverCallbackFunction, clickCallbackFunction, task, text,true);
    return element;
}

Element *guiAddSimpleSlider(
    List_Element *list,
    const Vec2i pos,
    const int width,
    const int height,
    const Vec3f colorBackground,
    const Vec3f colorSlider,
    SliderData* sliderData
)
{
    Element* element = guiAddElement(list, NULL, quadMesh, pos, width, height, NULL, colorBackground, (Padding){10, 10, 10, 10}, isSelected_Quad, hoverCallbackFunction, NULL, (Task){}, NULL,true);
    Vec2i sliderPos = {};
    sliderPos.x = width/2;
    sliderPos.y = height;
    Element* sliderElement = guiAddElement(list, NULL, quadMesh, pos, width, height, NULL, colorSlider, (Padding){10, 10, 10, 10}, isSelected_Quad, hoverCallbackFunction, sliderCallbackFunction, (Task){}, NULL,true);
    element->childElements.add(&element->childElements, sliderElement);
    element->ElementData = sliderData;
    return element;
}

Element *createElement(const ElementSettings elementSettings) {
    return guiAddElement(g_Elements,
                        elementSettings.name,
                        quadMesh,
                        elementSettings.pos,
                        elementSettings.width,
                        elementSettings.height,
                        elementSettings.texture,
                        elementSettings.color,
                        elementSettings.padding,
                        isSelected_Quad,
                        elementSettings.onHover,
                        elementSettings.onClick,
                        elementSettings.task,
                        elementSettings.text,true
    );
}




