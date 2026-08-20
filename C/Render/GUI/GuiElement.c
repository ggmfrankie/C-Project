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
#include "Utils/Makros/Makros.h"
#include "Utils/DataStructures/CArrayList.h"
#include "Utils/DataStructures/CHashMap.h"
#include "Utils/DataStructures/CSparseSet.h"

static SparseSet gElements;
static ElementHandle* gmElements;

void Element_init() {
    gElements = SparseSet_new(Element, 512);
}

ElementHandle Element_allocateNewV2(const Vec2i pos, const int width, const int height) {
    const ElementHandle handle = {
        .ID = SparseSet_add(&gElements, (Element){
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
                        .requestMove = nullptr
                      },
                    .visuals = {
                        .brightness = 1.0f,
                        .texture = {},
                        .transparency = 0,
                      },
                  .textElement = {.aCharQuads = nullptr, .textScale = 1.0f},
                  .parentElement = 0,
                  .aFlowElements = nullptr,
                  .padding = {0, 0, 0, 0},
                  .flags = {.isActive = true, .needsDeletion = true},
                  .task = (Task){nullptr, nullptr},
                  .childGap = 0,
                  .elementData = nullptr,
                  .positionMode = POS_FIT,
                  .layoutDirection = 0,
                  .type = 0,
                  .generateMesh = Mesh_generateRoundedCornerMesh
        })
    };
    Element_get(handle)->handle = handle;
    return handle;
}

ElementHandle Element_addChildElements(Element *parent, ...) {
    va_list args;
    va_start(args, parent);
    Element_addChildElements_vaList(parent->handle, args);
    va_end(args);
    return parent->handle;
}

ElementHandle Element_addChildElements_vaList(ElementHandle parentHandle, va_list args) {
    assert(parentHandle.ID != -1);
    while (1) {
        const ElementHandle childHandle = va_arg(args, ElementHandle);
        if (childHandle.ID == -1) break;

        Element* element = Element_get(childHandle);

        element->parentElement = parentHandle;
        Element* parent = Element_get(parentHandle);
        switch (element->positionMode) {
            case POS_FIT:
                arrPush(parent->aFlowElements, childHandle);
                break;
            case POS_RELATIVE:
                arrPush(parent->aStaticElements, childHandle);
                break;
        }
    }
    return parentHandle;
}

Element* Element_get(ElementHandle handle) {
    return SparseSet_get(&gElements, handle.ID, Element);
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

void Element_setText_ptr(Element* element, const char* text) {
    assert(element != nullptr);
    Strings.copyInto(&element->textElement.text, text);
    element->textElement.hasText = true;
    reloadTextQuads(getFont(), element);
}

void Element_setText_int(Element* element, const int i) {
    assert(element != nullptr);
    char tempText[512];
    Strings.fromInt(tempText, 512, i);
    Element_setText_ptr(element, tempText);
}

void Element_setActive_ptr(Element* element, const bool b) {
    assert(element != nullptr);
    element->flags.isActive = b;
}

void Element_toggleVisible_ptr(Element* element) {
    assert(element != nullptr);
    element->flags.isActive = !element->flags.isActive;
}

void Element_setColor_ptr(Element* element, const Vec3f color) {
    assert(element != nullptr);
    element->visuals.color = color;
}

Element *Element_getElement_ptr(const char *name) {
    assert(name != nullptr);
    Element* out = Element_get(*mapGet(gmElements, name));
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

ElementHandle Element_addElement(
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
    const ElementHandle handle = Element_allocateNewV2(pos, width, height);
    Element* lastElement = Element_get(handle);

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
        mapInsert(gmElements, name, handle);
    }

    if (text) {
        TextElement* t = &lastElement->textElement;
        t->hasText = true;
        t->aCharQuads = nullptr;
        t->text = newReservedString(128),
        t->textColor = (Vec3f){.0f, .0f, .0f};
        t->forceResize = forceResize,
        t->pos = (Vec2f){};
        t->width = 0;
        Element_setText_ptr(lastElement, text);
        reloadTextQuads(getFont(), lastElement);
    }
    return handle;
}

ElementHandle createElement(const ElementSettings es) {
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

ElementHandle _Element_new(ElementSettings es, ...) {
    const ElementHandle element = createElement(es);
    va_list args;
    va_start(args, es);
    Element_addChildElements_vaList(element, args);
    va_end(args);
    return element;
}

static ElementHandle defaultGenerator(int, int, ElementSettings es) {
    return createElement(es);
}

ElementHandle addChildrenAsGrid(const ElementSettings parentData, const ElementSettings es, const int numX, const int numY) {
    return addChildrenAsGridWithGenerator(parentData, es, numX, numY, defaultGenerator);
}

ElementHandle addChildrenAsGridWithGenerator(const ElementSettings parentData, ElementSettings es, const int numX, const int numY, ElementHandle (*generateElement)(int, int, ElementSettings)) {
    ElementHandle parentHandle = createElement(parentData);
    Element* parent = Element_get(parentHandle);
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
    return parentHandle;
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