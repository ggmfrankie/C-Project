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


Element elementArray[1024];
auto g_Elements = (List_Element){.content = elementArray, .capacity = 1024, .size = 0};
Hashmap_Element g_Hashmap = {};

void initElements() {
    g_Hashmap = newHashmap_Element(512);
}

Element* newElement(const Mesh mesh, const Vec2i pos, const int width, const int height) {
    g_Elements.content[g_Elements.size] = (Element){
        .name = nullptr,


        .dims = {
            .height = height,
            .pos = pos,
            .worldPos = pos,
            .actualWidth = width,
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
        .textElement = (TextElement){.charQuads = Character_newList(16), .forceResize = false, .hasText = false, .pos = {}, .text = NULL, .textColor = {}, .textScale = 1.0f, .width = 0},
        .parentElement = nullptr,
        .childElements = ChildElements_newList(8),
        .padding = (Padding){0,0,0,0},
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
    rebuildQuadMesh(element);
    pthread_mutex_unlock(&guiMutex);
}

void setText_noLock(Element* element, const char* text) {
    Strings.copyInto(&element->textElement.text, text);
    element->textElement.hasText = true;
    reloadTextQuads(getFont(), element);
    rebuildQuadMesh(element);
}

void setText_int(Element* element, const int i) {
    char tempText[512];
    Strings.fromInt(tempText, 512, i);

    setText(element, tempText);
    rebuildQuadMesh(element);
}

void setVisible(Element* element, const bool b) {
    pthread_mutex_lock(&guiMutex);
    element->flags.isActive = b;
    rebuildQuadMesh(element);
    pthread_mutex_unlock(&guiMutex);
}

void toggleVisible(Element* element) {
    pthread_mutex_lock(&guiMutex);
    element->flags.isActive = !element->flags.isActive;
    rebuildQuadMesh(element);
    pthread_mutex_unlock(&guiMutex);
}

void setColor(Element* element, const Vec3f color) {
    pthread_mutex_lock(&guiMutex);
    element->color = (Vec4f){color.x, color.y, color.z, 1.0f};
    rebuildQuadMesh(element);
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
    const LayoutDirection layoutDirection,
    bool fixedWidth,
    bool fixedHeight,
    void (*whileSelected)(Element *element),
    bool draggable,
    void (*onUpdate)(Element *element),
    bool wantGrowHorizontal,
    bool wantGrowVertical,
    float transparency,
    char *texture, bool invisible
)
{
    Element* lastElement = newElement(mesh, pos, width, height);
    if (mouseOver) {
        lastElement->isMouseOver = mouseOver;
        if (hover) lastElement->onHover = hover;
        if (click) {
            lastElement->onClick = click;
            if (task.func) {
                lastElement->task = task;
                if (task.userdata == nullptr) lastElement->task.userdata = lastElement;
            }
        }
    }
    lastElement->color = (Vec4f){color.x, color.y, color.z, 1.0f};
    lastElement->defaultColor = color;
    lastElement->padding = padding;
    lastElement->name = name;
    lastElement->positionMode = positionMode;
    lastElement->childGap = childGap;
    lastElement->reset = defaultReset;
    lastElement->elementData = elementData;
    lastElement->layoutDirection = layoutDirection;
    lastElement->flags.fixedWidth = fixedWidth;
    lastElement->flags.fixedHeight = fixedHeight;
    lastElement->whileSelected = whileSelected;
    lastElement->flags.draggable = draggable;
    lastElement->onUpdate = onUpdate;
    lastElement->flags.wantGrowHorizontal = wantGrowHorizontal;
    lastElement->flags.wantGrowVertical = wantGrowVertical;
    lastElement->transparency = transparency;
    lastElement->flags.hasTexture = false;
    lastElement->flags.invisible = invisible;

    if (texture) {
        lastElement->texture = getTexture(texture);
    }

    rebuildQuadMesh(lastElement);

    if (notSelectable) lastElement->isMouseOver = nullptr;

    if (name) {
        Hashmap_Element_add(&g_Hashmap, name, lastElement);
    }

    if (text) {
        lastElement->textElement = (TextElement) {
            .text = newReservedString(128),
            .textScale = 1.0f,
            .textColor = (Vec3f){1.0f, 1.0f, 1.0f},
            .forceResize = forceResize,
            .charQuads = Character_newList(16),
            .pos = (Vec2f){},
            .width = 0
        };
        setText_noLock(lastElement, text);
        lastElement->textElement.hasText = true;
        reloadTextQuads(getFont(), lastElement);
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
    Element* element = guiAddElement(&g_Elements, nullptr, quadMesh, pos, width, height, tex, (Vec3f){0.6f, 0.6f, 0.6f}, (Padding){10, 10, 10, 10}, 10, NULL, NULL, NULL, (Task){NULL, NULL}, NULL, false, POS_FIT, NULL, false, L_down, false, false, NULL, false, NULL, false, false, 0.0f, NULL, true);
    return element;
}

Element *guiAddSimpleRectangle_Color(
    const Vec2i pos,
    const int width,
    const int height,
    const Vec3f color
)
{
    Element* element = guiAddElement(&g_Elements, nullptr, quadMesh, pos, width, height, nullptr, color, (Padding){10, 10, 10, 10}, 10, NULL, NULL, NULL, (Task){NULL, NULL}, NULL, false, POS_FIT, NULL, false, L_down, false, false, NULL, false, NULL, false, false, 0.0f, NULL, true);
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
    Element* element = guiAddElement(&g_Elements, nullptr, quadMesh, pos, width, height, tex, (Vec3f){0.6f, 0.6f, 0.6f}, (Padding){10, 10, 10, 10}, 10, isSelected_Quad, hoverAndDragFun, runTaskFun, task, text, true, POS_FIT, NULL, false, L_down, false, false, NULL, false, NULL, false, false, 0.0f, NULL, true);
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
    Element* element = guiAddElement(&g_Elements, nullptr, quadMesh, pos, width, height, nullptr, color, (Padding){10, 10, 10, 10}, 10, isSelected_Quad, hoverAndDragFun, runTaskFun, task, text, true, POS_FIT, NULL, false, L_down, false, false, NULL, false, NULL, false, false, 0.0f, NULL, true);
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
    Element* element = guiAddElement(&g_Elements, nullptr, quadMesh, pos, width, height, nullptr, colorBackground, (Padding){10, 10, 10, 10}, 10, isSelected_Quad, hoverAndDragFun, NULL, (Task){}, NULL, true, POS_FIT, NULL, false, L_down, false, false, NULL, false, NULL, false, false, 0.0f, NULL, true);
    Vec2i sliderPos = {};
    sliderPos.x = width/2;
    sliderPos.y = 0;
    Element* sliderElement = guiAddElement(&g_Elements, nullptr, quadMesh, sliderPos, width, height, nullptr, colorSlider, (Padding){10, 10, 10, 10}, 10, isSelected_Quad, hoverAndDragFun, sliderCallbackFun, (Task){}, NULL, true, POS_FIT, NULL, false, L_down, false, false, NULL, false, NULL, false, false, 0.0f, NULL, true);
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

Element *createElement(const ElementSettings es) {
    return guiAddElement(&g_Elements,
                         es.name,
                         quadMesh,
                         es.pos,
                         es.width,
                         es.height,
                         es.old_texture,
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
                         es.invisible
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
    element->brightness = 1.0f;
}