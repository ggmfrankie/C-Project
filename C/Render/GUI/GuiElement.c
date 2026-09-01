//
// Created by Stefan on 28.10.2025.
//

#include "../GUI/GuiElement.h"

#include <float.h>
#include <pthread.h>
#include <stdarg.h>
#include "Render/Drawing/TextDisplaying.h"
#include "CallbackFunctions.h"
#include "../Drawing/Render.h"
#include "../Engine.h"
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

static ElementHandle Element_allocateNewV2(const Vec2f pos, const int width, const int height) {
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
                        .maxWidth = FLT_MAX,
                        .maxHeight = FLT_MAX
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
                        .texture = nullptr,
                        .transparency = 0,
                      },
                  .textElement = {.aCharQuads = nullptr, .scale = 1.0f},
                  .parentElement = 0,
                  .aFlowElements = nullptr,
                  .padding = {0, 0, 0, 0},
                  .flags = {.isActive = true},
                  .task = (Task){nullptr, nullptr},
                  .childGap = 0,
                  .elementData = nullptr,
                  .positionMode = POS_FIT,
                  .layoutDirection = 0,
                  .type = 0,
                  .generateMesh = nullptr
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

void Element_setOnClickCallback(Element* element, bool (*onClick)(Element* element)) {
    assert(element != nullptr);
    element->callbacks.onClick = onClick;
}

void Element_setOnHoverCallback(Element* element, bool (*onHover)(Element* element)) {
    assert(element != nullptr);
    element->callbacks.onHover = onHover;
}

void Element_setBoundingBox(Element* element, bool (*isMouseOver)(const Element *element, Vec2f mousePos)) {
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

static bool Element_isQuadBB(const Element *element, Vec2f mousePos) {
    if (mousePos.x <= element->dims.worldPos.x+element->dims.worldWidth && mousePos.x >= element->dims.worldPos.x &&
        mousePos.y <= element->dims.worldPos.y+element->dims.worldHeight && mousePos.y >= element->dims.worldPos.y) {
        return true;
    }
    return false;
}

ElementHandle createElement(const ElementSettings es) {
    const ElementHandle handle = Element_allocateNewV2(es.pos, es.minWidth, es.minHeight);
    Element* lastElement = Element_get(handle);

    if (!es.canNotBeSelected) {
        lastElement->callbacks.isMouseOver = Element_isQuadBB;
        if (es.onHover) lastElement->callbacks.onHover = es.onHover;
        if (es.onClick) {
            lastElement->callbacks.onClick = es.onClick;
            if (es.task.func) {
                lastElement->task = es.task;
                if (es.task.userdata == nullptr) lastElement->task.userdata = lastElement;
            }
        }
    }

    lastElement->visuals.color = es.color;
    lastElement->visuals.defaultColor = es.color;
    lastElement->padding = es.padding;
    lastElement->name = es.name;
    lastElement->positionMode = es.posMode;
    lastElement->childGap = es.childGap;
    lastElement->elementData.ptr = es.elementData;
    lastElement->layoutDirection = es.layoutDirection;
    lastElement->callbacks.whileSelected = es.whileSelected;
    lastElement->callbacks.onUpdate = es.onUpdate;
    lastElement->flags.wantGrowHorizontal = es.wantGrowHorizontal;
    lastElement->flags.wantGrowVertical = es.wantGrowVertical;
    lastElement->visuals.transparency = es.transparency;
    lastElement->visuals.brightness = 1.0f;
    lastElement->dims.cornerRadius = es.cornerRadius;
    lastElement->dims.flexGrow = es.flexGrow;
    lastElement->flags.canBeHovered = es.canBeHovered;
    lastElement->callbacks.drawCustom = es.drawCustom;

    if (!es.invisible) {
        lastElement->generateMesh = Mesh_generateRoundedCorner;
    }

    if (es.draggable) {
        lastElement->callbacks.requestMove = onRequestMove_SimpleDrag;
    }

    if (es.maxWidth) {
        lastElement->dims.maxWidth = es.maxWidth;
    }

    if (es.maxHeight) {
        lastElement->dims.maxHeight = es.maxHeight;
    }

    lastElement->visuals.texture = (es.texture) ?: "White.png";

    if (es.name) {
        mapInsert(gmElements, es.name, handle);
    }

    if (es.text) {
        TextElement* t = &lastElement->textElement;
        t->hasText = true;
        t->aCharQuads = nullptr;
        t->text = newReservedString(128),
        t->textColor = (Vec4f){.0f, .0f, .0f, 1.0f};
        t->forceResize = true,
        t->pos = (Vec2f){};
        t->width = 0;
        t->scale = es.textScale ? es.textScale : 1.0f;
        Element_setText_ptr(lastElement, es.text);
        reloadTextQuads(getFont(), lastElement);
    }
    return handle;
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
    printf("World pos is: %f, %f, Relative pos is: %f, %f\n dims = %f, %f, worldDims = %f, %f\n",
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