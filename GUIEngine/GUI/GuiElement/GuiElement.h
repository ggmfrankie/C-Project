//
// Created by Stefan on 28.10.2025.
//
#pragma once
#include "GuiDefines.h"
#include "GuiTypes.h"
#include "IO/Tasks.h"
#include "ElementTypes.h"

#include "Utils/Math/Vector.h"
#include "GUI/Drawing/Text.h"
#include "Utils/Typedef.h"

typedef struct Padding {
    float left;
    float up;
    float right;
    float down;
} Padding;

typedef enum PositionMode: byte {
    POS_FIT = 0,
    POS_RELATIVE
} PositionMode;

typedef enum LayoutDirection: byte {
    LAYOUT_DOWN = 0,
    LAYOUT_RIGHT
} LayoutDirection;

typedef enum UIState: byte {
    UI_STATE_NORMAL,
    UI_STATE_HOVER,
    UI_STATE_SELECTED,
    UI_STATE_DISABLED
} UIState;

typedef struct {
    int start;
    int end;

    struct {
        Vec2f start;
        Vec2f end;
    } rect;
} Line;

typedef struct {
    float minWidth;
    float minHeight;

    //layout lines
    Line* aLines;
} Cache;

typedef struct ElementHandle {
    ssize_t ID;
} ElementHandle;

typedef struct Element {
    const char* name;
    ElementType type;

    struct {
        bool isActive: 1;
        bool canBeHovered: 1;

        bool fixedWidth: 1;
        bool fixedHeight: 1;
        bool relativeWidth: 1;
        bool relativeHeight: 1;

        bool grow: 1;

        bool noLayoutContributionHorizontal: 1;
        bool noLayoutContributionVertical: 1;
        bool useClipping: 1;

        bool isHovered: 1;
        bool isSelected: 1;
    } flags;

    struct {
        Vec2f pos;
        Vec2f worldPos;

        float width;
        float height;
        float maxWidth;
        float maxHeight;

        float worldWidth;
        float worldHeight;

        float cornerRadius;

        float flexGrow;
    } dims;

    struct {
        const char* texture;

        Vec3f color;
        Vec3f defaultColor;

        float transparency;
        float brightness;
    } visuals;

    Cache layoutCache;

    ElementHandle handle;

    void (*generateMesh)(const Element* element, GuiVertex** aVertices, int** aIndices, ssize_t id);

    struct {
        bool (*isMouseOver)(const Element* element, Vec2f mousePos);
        bool (*onClick)(Element* element);
        bool (*onHover)(Element* element);
        void (*onLayoutUpdate)(Element* self);
        void (*whileSelected)(Element* element, double deltaTime);
        void (*onUpdate)(Element* element);
        void (*requestMove)(Element* element, Vec2f pos);
        void (*drawCustom)(const Element* element, GuiVertex** aVertices, int** aIndices, MeshInstanceData** additional, ssize_t ownerId);
    } callbacks;

    Padding padding;
    TextElement textElement;

    Task task;
    ElementHandle parentElement;
    ElementHandle* aFlowElements;
    ElementHandle* aStaticElements;
    float childGap;

    struct {
        void* ptr;
        void (*destructor)(void* data);
    } elementData;

    PositionMode positionMode;
    LayoutDirection layoutDirection;
} Element;


typedef struct ElementSettings {
    const char* name;
    Vec2f pos;
    PositionMode posMode;
    LayoutDirection layoutDirection;

    float cornerRadius;
    float minWidth;
    float minHeight;
    float maxWidth;
    float maxHeight;
    float flexGrow;

    char* texture;
    Vec3f color;
    Vec3f textColor;
    float transparency;

    char* text;
    float textScale;

    bool (*onClick)(Element* self);
    bool (*onHover)(Element* self);
    void (*whileSelected)(Element* self, double deltaTime);
    void (*onUpdate)(Element* element);
    void (*drawCustom)(const Element* self, GuiVertex** aVertices, int** aIndices, MeshInstanceData** additional, ssize_t ownerId);

    Task task;
    Padding padding;
    float childGap;
    bool canBeHovered;
    bool autoFit;
    bool invisible;
    bool notSelectable;
    bool draggable;
    bool grow;
    bool noLayoutContributionHorizontal;
    bool noLayoutContributionVertical;
    bool useClipping;

    struct {
        void* ptr;
        void (*destructor)(void* data);
    } elementData;

} ElementSettings;

void Element_init();
ElementHandle Element_addChildElements(Element *parent, ...);

ElementHandle Element_addChildElements_vaList(ElementHandle parentHandle, va_list args);

Element* Element_get(ElementHandle handle);
void Element_delete(ElementHandle selfHandle);

ElementHandle addChildrenAsGrid(ElementSettings parentData, ElementSettings es, int numX, int numY);
ElementHandle addChildrenAsGridWithGenerator(ElementSettings parentData, ElementSettings es, int numX, int numY, ElementHandle (*generateElement)(int row, int col, ElementSettings));

void Element_setOnClickCallback(Element* element, bool (*onClick)(Element* element));
void Element_setOnHoverCallback(Element* element, bool (*onHover)(Element* element));
void Element_setBoundingBox(Element* element, bool (*isMouseOver)(const Element *element, Vec2f mousePos));

ElementHandle Element_getElement(const char *name);
void Element_setText_ptr(Element* self, const char* text);
void Element_setText(ElementHandle selfHandle, const char* text);
void Element_setTextF(ElementHandle selfHandle, const char* fmt, ...);
void Element_setText_va(Element* element, const char* fmt, va_list args);
void Element_setActive(ElementHandle selfHandle, bool b);
void Element_toggleVisible(ElementHandle selfHandle);
void Element_setColor(ElementHandle selfHandle, Vec3f color);

void Element_printDebug(const Element* element);


ElementHandle createElement(ElementSettings es);

ElementHandle _Element_new(ElementSettings es, ...);

#define Element_new(...) _Element_new(__VA_ARGS__, (ElementHandle){-1})
#define Element_addChildren(...) Element_addChildElements(__VA_ARGS__, (ElementHandle){-1})