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
#include "../../../../../../Programme/CLion 2025.2.3/bin/mingw/x86_64-w64-mingw32/include/assert.h"


Element elementArray[1024];
List_Element g_Elements = (List_Element){.content = elementArray, .capacity = 1024, .size = 0};
Hashmap_Element g_Hashmap = {};

void initElements() {
    printf("Address of elementList: %p\n", &g_Elements);
    g_Hashmap = newHashmap_Element(512);
}

Element* newElement(const Mesh mesh, const Vec2i pos, const int width, const int height, Texture* texture) {
    g_Elements.content[g_Elements.size] = (Element){};
    g_Elements.content[g_Elements.size] = (Element){
        .name = NULL,
        .Mesh = mesh,
        .onClick = NULL,
        .onHover = NULL,
        .isMouseOver = NULL,
        .pos = pos,
        .worldPos = pos,
        .width = width,
        .height = height,
        .actualWidth = width,
        .actualHeight = height,
        .simpleTexture = texture,
        .color = (Vec3f){0.0f, 0.0f, 0.0f},
        .defaultColor = (Vec3f){0.0f, 0.0f, 0.0f},
        .textElement = (TextElement){.charQuads = Character_newList(16), .forceResize = false, .hasText = false, .pos = {}, .text = NULL, .textColor = {}, .textScale = 1.0f, .width = 0},
        .parentElement = NULL,
        .childElements = ChildElements_newList(8),
        .padding = (Padding){0,0,0,0},
        .flags = {.bits = {.isActive = true, .autoFit = true, .needsDeletion = true}},
        .task = (Task){NULL, NULL},
        .childGap = 0,
        .elementData = NULL,
        .positionMode = POS_FIT,
        .brightness = 1.0f,
        .guiMesh = {},
        .layoutDirection = 0,
        .onUpdate = NULL,
        .reset = NULL,
        .texture = {},
        .transparency = 0,
        .type = 0,
        .whileSelected = NULL
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
    element->onClick = onClick;
}

void setOnHoverCallback(Element* element, bool (*onHover)(Element* element, Renderer* renderer)) {
    element->onHover = onHover;
}

void setBoundingBox(Element* element, bool (*isMouseOver)(const Element *element, Vec2i mousePos)) {
    element->isMouseOver = isMouseOver;
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

void setVisible(Element* element, const bool b) {
    pthread_mutex_lock(&guiMutex);
    element->flags.bits.isActive = b;
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

void buildQuadMesh(Element* element) {
    GuiMesh *gm = &element->guiMesh;
    gm->length = 6;

    for (int i = 0; i < 6; i++) {
        gm->vertices[i] = (GuiVertex){
            .hasTexture = element->flags.bits.hasTexture,
            .brightness = 1.0f,
            .color = (Vec4f){element->color.x, element->color.y, element->color.z, 1.0f},
        };
    }

    gm->vertices[0].pos = (Vec2f){0.0f, 0.0f};
    gm->vertices[0].uv = element->texture.uv0;

    gm->vertices[1].pos = (Vec2f){0.0f, 1.0f};
    gm->vertices[1].uv = (Vec2f){element->texture.uv0.x, element->texture.uv1.y};

    gm->vertices[2].pos = (Vec2f){1.0f, 0.0f};
    gm->vertices[2].uv = (Vec2f){element->texture.uv1.x, element->texture.uv0.y};

    gm->vertices[3].pos = (Vec2f){1.0f, 0.0f};
    gm->vertices[3].uv = (Vec2f){element->texture.uv1.x, element->texture.uv0.y};

    gm->vertices[4].pos = (Vec2f){0.0f, 1.0f};
    gm->vertices[4].uv = (Vec2f){element->texture.uv0.x, element->texture.uv1.y};

    gm->vertices[5].pos = (Vec2f){1.0f, 1.0f};
    gm->vertices[5].uv = (Vec2f){element->texture.uv1.x, element->texture.uv1.y};
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
    char *texture
)
{
    assert(list == &g_Elements);
    Element* lastElement = newElement(mesh, pos, width, height, tex);
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
    lastElement->flags.bits.fixedWidth = fixedWidth;
    lastElement->flags.bits.fixedHeight = fixedHeight;
    lastElement->whileSelected = whileSelected;
    lastElement->flags.bits.draggable = draggable;
    lastElement->onUpdate = onUpdate;
    lastElement->flags.bits.wantGrowHorizontal = wantGrowHorizontal;
    lastElement->flags.bits.wantGrowVertical = wantGrowVertical;
    lastElement->transparency = transparency;
    lastElement->flags.bits.hasTexture = false;

    if (texture) {
        lastElement->texture = getTexture(texture);
    }

    buildQuadMesh(lastElement);

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
    Element* element = guiAddElement(&g_Elements, NULL, quadMesh, pos, width, height, tex, (Vec3f){0.6f, 0.6f, 0.6f}, (Padding){10, 10, 10, 10}, 10, NULL, NULL, NULL, (Task){NULL, NULL}, NULL, false, POS_FIT, NULL, false, L_down, false, false, NULL, false, NULL, false, false, 0.0f,NULL);
    return element;
}

Element *guiAddSimpleRectangle_Color(
    const Vec2i pos,
    const int width,
    const int height,
    const Vec3f color
)
{
    Element* element = guiAddElement(&g_Elements, NULL, quadMesh, pos, width, height, NULL, color, (Padding){10, 10, 10, 10}, 10, NULL, NULL, NULL, (Task){NULL, NULL}, NULL, false, POS_FIT, NULL, false, L_down, false, false, NULL, false, NULL, false, false, 0.0f,NULL);
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
    Element* element = guiAddElement(&g_Elements, NULL, quadMesh, pos, width, height, tex, (Vec3f){0.6f, 0.6f, 0.6f}, (Padding){10, 10, 10, 10}, 10, isSelected_Quad, hoverAndDragFun, runTaskFun, task, text, true, POS_FIT, NULL, false, L_down, false, false, NULL, false, NULL, false, false, 0.0f,NULL);
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
    Element* element = guiAddElement(&g_Elements, NULL, quadMesh, pos, width, height, NULL, color, (Padding){10, 10, 10, 10}, 10, isSelected_Quad, hoverAndDragFun, runTaskFun, task, text, true, POS_FIT, NULL, false, L_down, false, false, NULL, false, NULL, false, false, 0.0f,NULL);
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
    Element* element = guiAddElement(&g_Elements, NULL, quadMesh, pos, width, height, NULL, colorBackground, (Padding){10, 10, 10, 10}, 10, isSelected_Quad, hoverAndDragFun, NULL, (Task){}, NULL, true, POS_FIT, NULL, false, L_down, false, false, NULL, false, NULL, false, false, 0.0f,NULL);
    Vec2i sliderPos = {};
    sliderPos.x = width/2;
    sliderPos.y = 0;
    Element* sliderElement = guiAddElement(&g_Elements, NULL, quadMesh, sliderPos, width, height, NULL, colorSlider, (Padding){10, 10, 10, 10}, 10, isSelected_Quad, hoverAndDragFun, sliderCallbackFun, (Task){}, NULL, true, POS_FIT, NULL, false, L_down, false, false, NULL, false, NULL, false, false, 0.0f,NULL);
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
                         es.transparency,NULL
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