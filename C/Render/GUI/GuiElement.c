//
// Created by Stefan on 28.10.2025.
//

#include "../GUI/GuiElement.h"

#include <pthread.h>
#include <stdarg.h>
#include "Render/Drawing/TextDisplaying.h"
#include "CallbackFunctions.h"
#include "../Drawing/Render.h"
#include "../Engine.h"
#include "GuiElementData.h"
#include "Render/Drawing/Mesh.h"
#include "Utils/Makros.h"
#include "Utils/CArrayList.h"
#include "Utils/CHashMap.h"
static constexpr int MAX_ELEMENTS = 1024;
typedef struct {
    Element m[MAX_ELEMENTS];
    size_t size;
} ElementList;

static Element** g_map_Elements;
static ElementList g_Elements;

Element* Element_allocateNew(const Vec2i pos, const int width, const int height) {
    g_Elements.m[g_Elements.size] = (Element){
        .name = nullptr,
        .state = UI_STATE_NORMAL,
        .dims = {
            .width = width,
            .height = height,
            .pos = pos,
            .worldPos = pos,
            .worldWidth = width,
            .worldHeight = height,
            .maxWidth = INT32_MAX,
            .maxHeight = INT32_MAX
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
        .textElement = {.aCharQuads = nullptr, .textScale = 1.0f},
        .parentElement = nullptr,
        .aFlowElements = nullptr,
        .padding = {0,0,0,0},
        .flags = {.isActive = true, .needsDeletion = true},
        .task = (Task){nullptr, nullptr},
        .childGap = 0,
        .elementData = nullptr,
        .positionMode = POS_FIT,
        .layoutDirection = 0,
        .type = 0,
        .ID = (int)g_Elements.size,
        .generateMesh = Mesh_loadRoundedCornerMesh2,
    };
    assert(g_Elements.size < MAX_ELEMENTS);
    return &g_Elements.m[g_Elements.size++];
}

Element* Element_addChildElements(Element* parent, ...) {
    va_list args;
    va_start(args, parent);
    Element_addChildElements_vaList(parent, args);
    va_end(args);
    return parent;
}

Element* Element_addChildElements_vaList(Element* parent, va_list args) {
    assert(parent != nullptr);
    while (1) {
        Element* child = va_arg(args, Element*);

        if (child == nullptr) break;

        child->parentElement = parent;
        switch (child->positionMode) {
            case POS_FIT:
                arrPush(parent->aFlowElements, child);
                break;
            case POS_RELATIVE:
                arrPush(parent->aStaticElements, child);
                break;
        }
    }
    return parent;
}

void Element_setOnClickCallback(Element* element, bool (*onClick)(Element* element, Renderer* renderer)) {
    assert(element != nullptr);
    element->callbacks.onClick = onClick;
}

void Element_setOnHoverCallback(Element* element, bool (*onHover)(Element* element, Renderer* renderer)) {
    assert(element != nullptr);
    element->callbacks.onHover = onHover;
}

void Element_setBoundingBox(Element* element, bool (*isMouseOver)(const Element *element, Vec2i mousePos)) {
    assert(element != nullptr);
    element->callbacks.isMouseOver = isMouseOver;
}

void Element_setText(Element* element, const char* text) {
    assert(element != nullptr);
    Strings.copyInto(&element->textElement.text, text);
    element->textElement.hasText = true;
    reloadTextQuads(getFont(), element);
}

void Element_setText_int(Element* element, const int i) {
    assert(element != nullptr);
    char tempText[512];
    Strings.fromInt(tempText, 512, i);
    Element_setText(element, tempText);
}

void Element_setActive(Element* element, const bool b) {
    assert(element != nullptr);
    element->flags.isActive = b;
}

void Element_toggleVisible(Element* element) {
    assert(element != nullptr);
    element->flags.isActive = !element->flags.isActive;
}

void Element_setColor(Element* element, const Vec3f color) {
    assert(element != nullptr);
    element->visuals.color = color;
}

Element* Element_getElement(const char* name) {
    assert(name != nullptr);
    Element* out = *mapGet(g_map_Elements, name);
    assert(out != nullptr);
    return out;
}

bool Element_isQuadBB(const Element *element, const Vec2i mousePos) {
    if (mousePos.x <= element->dims.worldPos.x+element->dims.worldWidth && mousePos.x >= element->dims.worldPos.x &&
        mousePos.y <= element->dims.worldPos.y+element->dims.worldHeight && mousePos.y >= element->dims.worldPos.y) {
        return true;
    }
    return false;
}

Element *Element_addElement(
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
    int maxWidth,
    int maxHeight,
    void (*whileSelected)(Element *element),
    bool draggable,
    void (*onUpdate)(Element *element),
    bool wantGrowHorizontal,
    bool wantGrowVertical,
    float transparency,
    const char *texture,
    bool invisible,
    int cornerRadius,
    float flexGrow,
    bool canBeHovered
)
{
    Element* lastElement = Element_allocateNew(pos, width, height);
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

    const Padding p = {
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
    lastElement->callbacks.reset = Element_defaultReset;
    lastElement->elementData = elementData;
    lastElement->layoutDirection = layoutDirection;
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
    lastElement->dims.flexGrow = flexGrow;
    lastElement->flags.canBeHovered = canBeHovered;

    if (maxWidth) {
        lastElement->dims.maxWidth = maxWidth;
    }

    if (maxHeight) {
        lastElement->dims.maxHeight = maxHeight;
    }

    if (texture) {
        lastElement->visuals.texture = getTexture(texture);
        lastElement->flags.hasTexture = true;
    }

    if (notSelectable) lastElement->callbacks.isMouseOver = nullptr;

    if (name) {
        mapInsert(g_map_Elements, name, lastElement);
    }

    if (text) {
        const auto t = &lastElement->textElement;
        t->hasText = true;
        t->aCharQuads = nullptr;
        t->text = newReservedString(128),
        t->textColor = (Vec3f){.0f, .0f, .0f};
        t->forceResize = forceResize,
        t->pos = (Vec2f){};
        t->width = 0;
        Element_setText(lastElement, text);
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
    Element* element = Element_addElement(nullptr, pos, width, height, colorBackground, (Padding){10, 10, 10, 10}, 10, Element_isQuadBB, nullptr, NULL, (Task){}, NULL, true, POS_FIT, NULL, false, LAYOUT_DOWN, false, false, NULL, false, NULL, false, false, 0.0f, NULL, true, 0, 0.0f, false);
    Vec2i sliderPos = {};
    sliderPos.x = width/2;
    sliderPos.y = 0;
    Element* sliderElement = Element_addElement(nullptr, sliderPos, width, height, colorSlider, (Padding){10, 10, 10, 10}, 10, Element_isQuadBB, nullptr, sliderCallbackFun, (Task){}, NULL, true, POS_FIT, NULL, false, LAYOUT_DOWN, false, false, NULL, false, NULL, false, false, 0.0f, NULL, true, 0, 0.0f, false);
    arrPush(element->aFlowElements, sliderElement);
    element->elementData = sliderData;
    return element;
}

Element *TextFieldElement_new(const ElementSettings elementSettings, bool (*onEnterCallback)(Element* element, Renderer *renderer)) {
    Element* element = createElement(elementSettings);
    TextFieldData* textData = calloc(1, sizeof(TextFieldData));
    textData->onEnterCallback = onEnterCallback;
    Element* textField = createElement(
        (ElementSettings){
            .minWidth = elementSettings.minWidth,
            .minHeight = elementSettings.minHeight,
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

Element* createElement(const ElementSettings es) {
    return Element_addElement(es.name,
                              es.pos,
                              es.minWidth,
                              es.minHeight,
                              es.color,
                              es.padding,
                              es.childGap,
                              Element_isQuadBB,
                              es.onHover,
                              es.onClick,
                              es.task,
                              es.text,
                              true,
                              es.posMode,
                              es.elementData,
                              es.cantBeSelected,
                              es.layoutDirection,
                              es.maxWidth,
                              es.maxHeight,
                              es.whileSelected,
                              es.draggable,
                              es.onUpdate,
                              es.wantGrowHorizontal,
                              es.wantGrowVertical,
                              es.transparency,
                              nullptr,
                              es.invisible,
                              es.cornerRadius,
                              es.flexGrow,
                              es.canBeHovered
    );
}

Element* _Element_new(ElementSettings es, ...) {
    Element* element = createElement(es);
    va_list args;
    va_start(args, es);
    Element_addChildElements_vaList(element, args);
    va_end(args);
    return element;
}

static Element* defaultGenerator(int, int, ElementSettings es) {
    return createElement(es);
}

Element* addChildrenAsGrid(const ElementSettings parentData, const ElementSettings es, const int numX, const int numY) {
    return addChildrenAsGridWithGenerator(parentData, es, numX, numY, defaultGenerator);
}

Element* addChildrenAsGridWithGenerator(const ElementSettings parentData, ElementSettings es, const int numX, const int numY, Element* (*generateElement)(int, int, ElementSettings)) {
    Element* parent = createElement(parentData);
    const int childWidth = parent->dims.width/numX;
    const int childHeight = parent->dims.height/numY;

    es.posMode = POS_RELATIVE;
    es.minWidth = childWidth;
    es.minHeight = childHeight;

    for (int i = 0; i < numX; i++) {
        for (int ii = 0; ii < numY; ii++) {
            es.pos.x = (parentData.childGap + childWidth) * i;
            es.pos.y = (parentData.childGap + childHeight) * ii;
            addChildElements(parent, generateElement(i, ii, es));
        }
    }
    return parent;
}

void Element_defaultReset(Element* element) {
    element->visuals.brightness = 1.0f;
}

void Element_printDebug(const Element* element) {
    printf("World pos is: %i, %i, Relative pos is: %i, %i\n dims = %i, %i, worldDims = %i, %i\n",
           element->dims.worldPos.x,
           element->dims.worldPos.y,
           element->dims.pos.x,
           element->dims.pos.y,
           element->dims.width,
           element->dims.height,
           element->dims.worldWidth,
           element->dims.worldWidth
    );
}