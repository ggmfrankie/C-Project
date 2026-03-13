//
// Created by Stefan on 28.10.2025.
//

#include "../GUI/GuiElement.h"

#include <pthread.h>
#include <stdarg.h>

#include "CallbackFunctions.h"
#include "../Drawing/Render.h"
#include "../Engine.h"
#include "GuiElementData.h"
#include "Render/Drawing/Mesh.h"
#include "Utils/Makros.h"


Element elementArray[1024];
auto g_Elements = (List_Element){.content = elementArray, .capacity = 1024, .size = 0};
Hashmap_Element g_Hashmap = {};

void initElements() {
    g_Hashmap = newHashmap_Element(512);
}

Element* newElement(const Vec2i pos, const int width, const int height) {
    g_Elements.content[g_Elements.size] = (Element){
        .name = nullptr,
        .dims = {
            .width = width,
            .height = height,
            .pos = pos,
            .worldPos = pos,
            .actualWidth = width,
            .actualHeight = height
        },
        .callbacks = {
            .onClick = nullptr,
            .onHover = nullptr,
            .isMouseOver = nullptr,
            .onUpdate = nullptr,
            .reset = nullptr,
            .whileSelected = nullptr,
        },
        .visuals = {
            .brightness = 1.0f,
            .texture = {},
            .transparency = 0,
        },
        .textElement = {.charQuads = Character_newList(16), .forceResize = false, .hasText = false, .pos = {}, .text = NULL, .textColor = {}, .textScale = 1.0f, .width = 0},
        .parentElement = nullptr,
        .childElements = ChildElements_newList(8),
        .padding = {0,0,0,0},
        .flags = {.isActive = true, .autoFit = true, .needsDeletion = true},
        .task = (Task){nullptr, nullptr},
        .childGap = 0,
        .elementData = NULL,
        .positionMode = POS_FIT,
        .layoutDirection = 0,
        .type = 0,
        .ID = (int)g_Elements.size,
        .generateMesh = Mesh_loadRoundedCornerMesh2,
    };
    return &g_Elements.content[g_Elements.size++];
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
    element->callbacks.onClick = onClick;
}

void setOnHoverCallback(Element* element, bool (*onHover)(Element* element, Renderer* renderer)) {
    element->callbacks.onHover = onHover;
}

void setBoundingBox(Element* element, bool (*isMouseOver)(const Element *element, Vec2i mousePos)) {
    element->callbacks.isMouseOver = isMouseOver;
}

void setText(Element* element, const char* text) {
    pthread_mutex_lock(&guiMutex);
    Strings.copyInto(&element->textElement.text, text);
    element->textElement.hasText = true;
    reloadTextQuads(getFont(), element);
    pthread_mutex_unlock(&guiMutex);
}

void setText_noLock(Element* element, const char* text) {
    Strings.copyInto(&element->textElement.text, text);
    element->textElement.hasText = true;
    reloadTextQuads(getFont(), element);
}

void setText_int(Element* element, const int i) {
    char tempText[512];
    Strings.fromInt(tempText, 512, i);

    setText(element, tempText);
}

void setActive(Element* element, const bool b) {
    pthread_mutex_lock(&guiMutex);
    element->flags.isActive = b;
    pthread_mutex_unlock(&guiMutex);
}

void toggleVisible(Element* element) {
    pthread_mutex_lock(&guiMutex);
    element->flags.isActive = !element->flags.isActive;
    pthread_mutex_unlock(&guiMutex);
}

void Element_setColor(Element* element, const Vec3f color) {
    pthread_mutex_lock(&guiMutex);
    element->visuals.color = color;
    pthread_mutex_unlock(&guiMutex);
}

Element* getElement(const char* name) {
    return *Hashmap_Element_get(&g_Hashmap, name);
}

bool isSelected_Quad(const Element *element, const Vec2i mousePos) {
    if (mousePos.x <= element->dims.worldPos.x+element->dims.actualWidth && mousePos.x >= element->dims.worldPos.x &&
        mousePos.y <= element->dims.worldPos.y+element->dims.actualHeight && mousePos.y >= element->dims.worldPos.y) {
        return true;
    }
    return false;
}

Element *guiAddElement(
    char *name,
    const Vec2i pos,
    const int width,
    const int height,
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
    const LayoutDirection layoutDirection,
    bool fixedWidth,
    bool fixedHeight,
    void (*whileSelected)(Element *element),
    bool draggable,
    void (*onUpdate)(Element *element),
    bool wantGrowHorizontal,
    bool wantGrowVertical,
    float transparency,
    char *texture,
    bool invisible,
    int cornerRadius
)
{
    Element* lastElement = newElement(pos, width, height);
    if (mouseOver) {
        lastElement->callbacks.isMouseOver = mouseOver;
        if (hover) lastElement->callbacks.onHover = hover;
        if (click) {
            lastElement->callbacks.onClick = click;
            if (task.func) {
                lastElement->task = task;
                if (task.userdata == nullptr) lastElement->task.userdata = lastElement;
            }
        }
    }

    const auto p = (Padding){
        max(padding.left, cornerRadius),
        max(padding.up, cornerRadius),
        max(padding.right, cornerRadius),
        max(padding.up, cornerRadius),
    };

    lastElement->visuals.color = color;
    lastElement->visuals.defaultColor = color;
    lastElement->padding = p;
    lastElement->name = name;
    lastElement->positionMode = positionMode;
    lastElement->childGap = childGap;
    lastElement->callbacks.reset = defaultReset;
    lastElement->elementData = elementData;
    lastElement->layoutDirection = layoutDirection;
    lastElement->flags.fixedWidth = fixedWidth;
    lastElement->flags.fixedHeight = fixedHeight;
    lastElement->callbacks.whileSelected = whileSelected;
    lastElement->flags.draggable = draggable;
    lastElement->callbacks.onUpdate = onUpdate;
    lastElement->flags.wantGrowHorizontal = wantGrowHorizontal;
    lastElement->flags.wantGrowVertical = wantGrowVertical;
    lastElement->visuals.transparency = transparency;
    lastElement->flags.hasTexture = false;
    lastElement->flags.invisible = invisible;
    lastElement->visuals.brightness = 1.0f;
    lastElement->dims.cornerRadius = cornerRadius;

    if (texture) {
        lastElement->visuals.texture = getTexture(texture);
        lastElement->flags.hasTexture = true;
    }

    if (notSelectable) lastElement->callbacks.isMouseOver = nullptr;

    if (name) {
        Hashmap_Element_add(&g_Hashmap, name, lastElement);
    }

    if (text) {
        const auto t = &lastElement->textElement;
        t->hasText = true;
        t->charQuads = Character_newList(16);
        t->text = newReservedString(128),
        t->textColor = (Vec3f){.0f, .0f, .0f};
        t->forceResize = forceResize,
        t->pos = (Vec2f){};
        t->width = 0;
        setText_noLock(lastElement, text);
        reloadTextQuads(getFont(), lastElement);
    }
    return lastElement;
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
    Element* element = guiAddElement(nullptr, pos, width, height, colorBackground, (Padding){10, 10, 10, 10}, 10, isSelected_Quad, hoverAndDragFun, NULL, (Task){}, NULL, true, POS_FIT, NULL, false, L_down, false, false, NULL, false, NULL, false, false, 0.0f, NULL, true, 0);
    Vec2i sliderPos = {};
    sliderPos.x = width/2;
    sliderPos.y = 0;
    Element* sliderElement = guiAddElement(nullptr, sliderPos, width, height, colorSlider, (Padding){10, 10, 10, 10}, 10, isSelected_Quad, hoverAndDragFun, sliderCallbackFun, (Task){}, NULL, true, POS_FIT, NULL, false, L_down, false, false, NULL, false, NULL, false, false, 0.0f, NULL, true, 0);
    element->childElements.add(&element->childElements, sliderElement);
    element->elementData = sliderData;
    return element;
}

Element *createTextFieldElement(const ElementSettings elementSettings, bool (*onEnterCallback)(Element* element, Renderer *renderer)) {
    Element* element = createElement(elementSettings);
    TextFieldData* textData = calloc(1, sizeof(TextFieldData));
    textData->onEnterCallback = onEnterCallback;
    Element* textField = createElement(
        (ElementSettings){
            .width = elementSettings.width - elementSettings.padding.left - elementSettings.padding.right,
            .height = elementSettings.height - elementSettings.padding.up - elementSettings.padding.down,
            .padding = {5,5,5,5},
            .elementData = textData,
            .color = v_mul(elementSettings.color, 0.8f),
            .onClick = textField_onClick,
            .text = "",
            .task = elementSettings.task,
        }
    );
    textField->type = t_textField;
    element->type = t_textField;
    addChildElements(element, textField);

    return element;
}

Element *createElement(const ElementSettings es) {
    return guiAddElement(es.name,
                         es.pos,
                         es.width,
                         es.height,
                         es.color,
                         es.padding,
                         es.childGap,
                         isSelected_Quad,
                         es.onHover,
                         es.onClick,
                         es.task,
                         es.text,
                         true,
                         es.posMode,
                         es.elementData,
                         es.notSelectable,
                         es.layoutDirection,
                         es.fixedWidth,
                         es.fixedHeight,
                         es.whileSelected,
                         es.draggable,
                         es.onUpdate,
                         es.wantGrowHorizontal,
                         es.wantGrowVertical,
                         es.transparency,
                         nullptr,
                         es.invisible,
                         es.cornerRadius
    );
}

Element* addChildrenAsGrid(const ElementSettings parentData, const ElementSettings es, const int numX, const int numY) {
    return nullptr;
}

Element* addChildrenAsGridWithGenerator(const ElementSettings parentData, ElementSettings es, const int numX, const int numY, Element* (*generateElement)(int row, int col, ElementSettings)) {
    Element* parent = createElement(parentData);
    const int childWidth = parent->dims.width/numX;
    const int childHeight = parent->dims.height/numY;

    printf("childWidth>: %i childHeight>: %i\n", childWidth, childHeight);

    es.posMode = POS_ABSOLUTE;
    es.width = childWidth;
    es.height = childHeight;

    for (int i = 0; i < numX; i++) {
        for (int ii = 0; ii < numY; ii++) {
            es.pos.x = childWidth * i + parent->padding.left;
            es.pos.y = childHeight * ii + parent->padding.up;
            addChildElements(parent, generateElement(i, ii, es));
        }
    }
    return parent;
}

void defaultReset(Element* element) {
    element->visuals.brightness = 1.0f;
}