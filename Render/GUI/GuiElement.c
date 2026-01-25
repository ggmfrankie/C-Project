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


Element elementArray[1024];
List_Element g_Elements = (List_Element){.content = elementArray, .capacity = 1024, .size = 0};
Hashmap_Element g_Hashmap = {};

void initElements() {
    g_Hashmap = newHashmap_Element(512);
}

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
        .defaultColor = (Vec3f){0.0f, 0.0f, 0.0f},
        .textElement = NULL,
        .parentElement = NULL,
        .childElements = ChildElements_newList(8),
        .padding = (Padding){0,0,0,0},
        .hasText = false,
        .task = (Task){NULL, NULL},
        .autoFit = true,
        .childGap = 0,
        .elementData = NULL,
        .needsDeletion = false,
        .isActive = true,
        .positionMode = POS_FIT
    };
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

void setOnClickCallback(Element* element, bool (*onClick)(Element* element, Renderer* renderer)) {
    element->onClick = onClick;
}

void setOnHoverCallback(Element* element, bool (*onHover)(Element* element, Renderer* renderer)) {
    element->onHover = onHover;
}

void setBoundingBox(Element* element, bool (*isMouseOver)(const Element *element, Vec2i mousePos)) {
    element->isMouseOver = isMouseOver;
}

void copyText(Element* element, const char* text) {
    pthread_mutex_lock(&guiMutex);
    Strings.copyInto(&element->textElement.text, text);
    element->hasText = true;
    pthread_mutex_unlock(&guiMutex);
}

void setText(Element* element, char* text) {
    pthread_mutex_lock(&guiMutex);
    element->textElement.text = stringOf(text);
    element->hasText = true;
    pthread_mutex_unlock(&guiMutex);
}

void setText_int(Element* element, const int i) {
    char tempText[512];
    Strings.fromInt(tempText, 512, i);

    copyText(element, tempText);
}

void setVisible(Element* element, const bool b) {
    pthread_mutex_lock(&guiMutex);
    element->isActive = b;
    pthread_mutex_unlock(&guiMutex);
}

void setColor(Element* element, const Vec3f color) {
    pthread_mutex_lock(&guiMutex);
    element->color = color;
    pthread_mutex_unlock(&guiMutex);
}

Element* getElement(const char* name) {
    return *Hashmap_Element_get(&g_Hashmap, name);
}

bool isSelected_Quad(const Element *element, const Vec2i mousePos) {
    if (mousePos.x <= element->worldPos.x+element->actualWidth && mousePos.x >= element->worldPos.x &&
        mousePos.y <= element->worldPos.y+element->actualHeight && mousePos.y >= element->worldPos.y) {
        return true;
    }
    return false;
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
    const int childGap,
    bool (*mouseOver)(const Element *, Vec2i),
    bool (*hover)(Element *, Renderer *),
    bool (*click)(Element *, Renderer *),
    const Task task,
    const char *text,
    const bool forceResize,
    const PositionMode positionMode,
    void *elementData,
    const bool notSelectable,
    const LayoutDirection layoutDirection, bool fixedWidth, bool fixedHeight, void (*whileSelected)(Element *element), bool
    draggable, void (*onUpdate)(Element *element)
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
    lastElement->defaultColor = color;
    lastElement->padding = padding;
    lastElement->name = name;
    lastElement->positionMode = positionMode;
    lastElement->childGap = childGap;
    lastElement->reset = defaultReset;
    lastElement->elementData = elementData;
    lastElement->layoutDirection = layoutDirection;
    lastElement->fixedWidth = fixedWidth;
    lastElement->fixedHeight = fixedHeight;
    lastElement->whileSelected = whileSelected;
    lastElement->draggable = draggable;
    lastElement->onUpdate = onUpdate;

    if (notSelectable) lastElement->isMouseOver = NULL;

    if (name) {
        Hashmap_Element_add(&g_Hashmap, name, lastElement);
    }

    if (text) {
        lastElement->textElement = (TextElement) {
            .text = newReservedString(128),
            .textScale = 1.0f,
            .textColor = (Vec3f){1.0f, 1.0f, 1.0f},
            .forceResize = forceResize,
        };
        copyText(lastElement, text);
        lastElement->hasText = true;
    }
    return lastElement;
}

Element *guiAddSimpleRectangle_Texture(
    const Vec2i pos,
    const int width,
    const int height,
    Texture *tex
)
{
    Element* element = guiAddElement(&g_Elements, NULL, quadMesh, pos, width, height, tex, (Vec3f){0.6f, 0.6f, 0.6f}, (Padding){10, 10, 10, 10}, 10, NULL, NULL, NULL, (Task){NULL, NULL}, NULL, false, POS_FIT, NULL, false, L_down, false, false, NULL, false,NULL);
    return element;
}

Element *guiAddSimpleRectangle_Color(
    const Vec2i pos,
    const int width,
    const int height,
    const Vec3f color
)
{
    Element* element = guiAddElement(&g_Elements, NULL, quadMesh, pos, width, height, NULL, color, (Padding){10, 10, 10, 10}, 10, NULL, NULL, NULL, (Task){NULL, NULL}, NULL, false, POS_FIT, NULL, false, L_down, false, false, NULL, false,NULL);
    return element;
}

Element *guiAddSimpleButton_Texture(
    const Vec2i pos,
    const int width,
    const int height,
    Texture *tex,
    const Task task,
    const char *text
)
{
    Element* element = guiAddElement(&g_Elements, NULL, quadMesh, pos, width, height, tex, (Vec3f){0.6f, 0.6f, 0.6f}, (Padding){10, 10, 10, 10}, 10, isSelected_Quad, hoverAndDragFun, runTaskFun, task, text, true, POS_FIT, NULL, false, L_down, false, false, NULL, false,NULL);
    return element;
}

Element *guiAddSimpleButton_Color(
    const Vec2i pos,
    const int width,
    const int height,
    const Vec3f color,
    const Task task,
    const char *text
)
{
    Element* element = guiAddElement(&g_Elements, NULL, quadMesh, pos, width, height, NULL, color, (Padding){10, 10, 10, 10}, 10, isSelected_Quad, hoverAndDragFun, runTaskFun, task, text, true, POS_FIT, NULL, false, L_down, false, false, NULL, false,NULL);
    return element;
}

Element *guiAddSimpleSlider(
    const Vec2i pos,
    const int width,
    const int height,
    const Vec3f colorBackground,
    const Vec3f colorSlider,
    SliderData* sliderData
)
{
    Element* element = guiAddElement(&g_Elements, NULL, quadMesh, pos, width, height, NULL, colorBackground, (Padding){10, 10, 10, 10}, 10, isSelected_Quad, hoverAndDragFun, NULL, (Task){}, NULL, true, POS_FIT, NULL, false, L_down, false, false, NULL, false,NULL);
    Vec2i sliderPos = {};
    sliderPos.x = width/2;
    sliderPos.y = 0;
    Element* sliderElement = guiAddElement(&g_Elements, NULL, quadMesh, sliderPos, width, height, NULL, colorSlider, (Padding){10, 10, 10, 10}, 10, isSelected_Quad, hoverAndDragFun, sliderCallbackFun, (Task){}, NULL, true, POS_FIT, NULL, false, L_down, false, false, NULL, false,NULL);
    element->childElements.add(&element->childElements, sliderElement);
    element->elementData = sliderData;
    return element;
}

Element *createTextFieldElement(const ElementSettings elementSettings) {
    Element* element = createElement(elementSettings);
    TextFieldData* textData = calloc(1, sizeof(TextFieldData));
    Element* textField = createElement(
        (ElementSettings){
            .width = elementSettings.width - elementSettings.padding.left - elementSettings.padding.right,
            .height = elementSettings.height - elementSettings.padding.up - elementSettings.padding.down,
            .elementData = textData,
            .color = v_mul(elementSettings.color, 0.8f),
            .onClick = textFieldCallbackFun,
            .text = "hallo"
        }
    );
    textField->type = t_textField;
    addChildElements(element, textField);

    return element;
}

Element *createElement(const ElementSettings elementSettings) {
    return guiAddElement(&g_Elements,
                         elementSettings.name,
                         quadMesh,
                         elementSettings.pos,
                         elementSettings.width,
                         elementSettings.height,
                         elementSettings.texture,
                         elementSettings.color,
                         elementSettings.padding,
                         elementSettings.childGap,
                         isSelected_Quad,
                         elementSettings.onHover,
                         elementSettings.onClick,
                         elementSettings.task,
                         elementSettings.text,
                         true,
                         elementSettings.posMode,
                         elementSettings.elementData,
                         elementSettings.notSelectable,
                         elementSettings.layoutDirection,
                         elementSettings.fixedWidth,
                         elementSettings.fixedHeight,
                         elementSettings.whileSelected,
                         elementSettings.draggable,
                         elementSettings.onUpdate
    );
}

Element* addChildrenAsGrid(const ElementSettings parentData, const ElementSettings es, const int numX, const int numY) {
    return addChildrenAsGridWithGenerator(parentData, es, numX, numY, createElement);
}

Element* addChildrenAsGridWithGenerator(const ElementSettings parentData, ElementSettings es, const int numX, const int numY, Element* (*generateElement)(ElementSettings)) {
    Element* parent = createElement(parentData);
    const int childWidth = parent->width/numX;
    const int childHeight = parent->height/numY;

    es.posMode = POS_ABSOLUTE;
    es.width = childWidth;
    es.height = childHeight;

    for (int i = 0; i < numX; i++) {
        for (int ii = 0; ii < numY; ii++) {
            es.pos.x = childWidth * i + parent->padding.left;
            es.pos.y = childHeight * ii + parent->padding.up;
            addChildElements(parent, generateElement(es));
        }
    }
    return parent;
}

void defaultReset(Element* element) {
    element->state = 0;
}