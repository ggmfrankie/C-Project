//
// Created by Stefan on 28.10.2025.
//

#include "GuiElement.h"

#include <float.h>
#include <pthread.h>
#include <stdarg.h>
#include "CallbackHelper.h"
#include "../Engine.h"
#include "../Drawing/Mesh/Mesh.h"
#include "Utils/DataStructures/CArrayList.h"
#include "Utils/DataStructures/CHashMap.h"
#include "Utils/DataStructures/CSparseSet.h"

static SparseSet gElements;
static ElementHandle* gmElements;

void Element_init() {
    Log_info("Initializing element storage");
    Log_debug("SparseSet capacity: 512");
    gElements = SparseSet_new(Element, 512);
}

static ElementHandle Element_allocateNewV2(const Vec2f pos, const int width, const int height) {
    const ElementHandle handle = {
        .ID = SparseSet_add(&gElements, (Element){
                    .name = nullptr,
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

static void Element_deleteRec(ElementHandle selfHandle) {
    // first let children delete themselves

    for arrEach(childHandle, Element_get(selfHandle)->aFlowElements) {
        Element_deleteRec(*childHandle);
    }
    for arrEach(childHandle, Element_get(selfHandle)->aStaticElements) {
        Element_deleteRec(*childHandle);
    }

    Element* element = Element_get(selfHandle);

    if (element->elementData.ptr && element->elementData.needsFree) free(element->elementData.ptr);
    if (element->task.userdata && element->task.needsFree) free(element->task.userdata);
    if (element->textElement.sText) strFree(&element->textElement.sText);

    arrFree(element->aFlowElements);
    arrFree(element->aStaticElements);
    arrFree(element->layoutCache.aLines);

    SparseSet_remove_keepOrder(&gElements, selfHandle.ID);
}

void Element_delete(ElementHandle selfHandle) {
    // removes itself from the parent element
    bool found = false;
    Element* parent = Element_get(Element_get(selfHandle)->parentElement);
    arrRemoveIf(flowElement, parent->aFlowElements, ({found = flowElement->ID == selfHandle.ID;}));
    if (!found) arrRemoveIf(flowElement, parent->aFlowElements, ({flowElement->ID == selfHandle.ID;}));

    Element_deleteRec(selfHandle);
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

void Element_setText_ptr(Element* self, const char* text) {
    assert(element != nullptr);

    strClear(self->textElement.sText);
    strAppend_sprintf(&self->textElement.sText, text);

    self->textElement.hasText = true;
    Text_reloadTextQuads(self);
}

void Element_setText(ElementHandle selfHandle, const char* text) {
    Element_setText_ptr(Element_get(selfHandle), text);
}

void Element_setText_va(Element* element, const char* fmt, va_list args) {
    assert(element != nullptr);
    strClear(element->textElement.sText);
    strAppend_sprintfVa(&element->textElement.sText, fmt, args);
    va_end(args);
    element->textElement.hasText = true;
    Text_reloadTextQuads(element);
}

void Element_setTextF(ElementHandle selfHandle, const char* fmt, ...) {
    assert(element != nullptr);
    va_list args;
    va_start(args, fmt);
    Element_setText_va(Element_get(selfHandle), fmt, args);
    va_end(args);
}

void Element_setActive(ElementHandle selfHandle, const bool b) {
    assert(element != nullptr);
    Element_get(selfHandle)->flags.isActive = b;
}

void Element_toggleVisible(ElementHandle selfHandle) {
    assert(element != nullptr);
    Element* self = Element_get(selfHandle);
    self->flags.isActive = !self->flags.isActive;
}

void Element_setColor(ElementHandle selfHandle, const Vec3f color) {
    assert(element != nullptr);
    Element_get(selfHandle)->visuals.color = color;
}

ElementHandle Element_getElement(const char *name) {
    assert(name != nullptr);
    const ElementHandle out = *mapGet(gmElements, name);
    return out;
}

static bool Element_isQuadBB(const Element *element, Vec2f mousePos) {
    if (mousePos.x <= element->dims.worldPos.x+element->dims.worldWidth && mousePos.x >= element->dims.worldPos.x &&
        mousePos.y <= element->dims.worldPos.y+element->dims.worldHeight && mousePos.y >= element->dims.worldPos.y) {
        return true;
    }
    return false;
}

ElementHandle createElement(ElementSettings es) {
    const ElementHandle handle = Element_allocateNewV2(es.pos, es.minWidth, es.minHeight);
    Element* lastElement = Element_get(handle);

    if (!es.notSelectable) {
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
    lastElement->flags.grow = es.grow;
    lastElement->visuals.transparency = es.transparency;
    lastElement->visuals.brightness = 1.0f;
    lastElement->dims.cornerRadius = es.cornerRadius;
    lastElement->dims.flexGrow = es.flexGrow;
    lastElement->flags.canBeHovered = es.canBeHovered;
    lastElement->callbacks.drawCustom = es.drawCustom;

    lastElement->flags.noLayoutContributionHorizontal = es.noLayoutContributionHorizontal;
    lastElement->flags.noLayoutContributionVertical = es.noLayoutContributionVertical;

    lastElement->flags.useClipping = es.useClipping;

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
        t->sText = strNew(32),
        t->textColor = (Vec4f){es.textColor.x, es.textColor.y, es.textColor.z, 1.0f};
        t->forceResize = true,
        t->pos = (Vec2f){};
        t->width = 0;
        t->font = Engine_getDefaultFont();
        t->scale = es.textScale ? es.textScale : 1.0f;
        Element_setText_ptr(lastElement, es.text);
        Text_reloadTextQuads(lastElement);
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

ElementHandle addChildrenAsGridWithGenerator(ElementSettings parentData, ElementSettings es, const int numX, const int numY, ElementHandle (*generateElement)(int, int, ElementSettings)) {
    ElementHandle parentHandle = createElement(parentData);
    Element* parent            = Element_get(parentHandle);
    const int childWidth       = parent->dims.width/numX;
    const int childHeight      = parent->dims.height/numY;

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