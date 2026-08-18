//
// Created by Stefan on 28.10.2025.
//
#pragma once
#include "GuiDefines.h"
#include "GuiTypes.h"
#include "Tasks.h"

#include "Utils/Math/Vector.h"
#include "Render/Drawing/TextDisplaying.h"
#include "Utils/Typedef.h"

typedef struct Padding {
    int left;
    int up;
    int right;
    int down;
} Padding;

typedef enum PositionMode: byte {
    POS_FIT = 0,
    POS_RELATIVE
} PositionMode;

typedef enum LayoutDirection: byte {
    LAYOUT_DOWN = 0,
    LAYOUT_RIGHT
} LayoutDirection;

typedef enum ElementType: byte {
    t_defaultElement,
    t_slider,
    t_textField
} ElementType;

typedef enum UIState: byte {
    UI_STATE_NORMAL,
    UI_STATE_HOVER,
    UI_STATE_PRESSED,
    UI_STATE_SELECTED,
    UI_STATE_DISABLED
} UIState;

typedef struct {
    int start;
    int end;
} Line;

typedef struct {
    int minWidth;
    int minHeight;

    //layout lines
    Line* aLines;
} Cache;

typedef struct ElementHandle {
    ssize_t ID;
} ElementHandle;

typedef struct Element {
    char* name;
    ElementType type;
    UIState state;

    struct {
        bool isActive: 1;
        bool invisible: 1;
        bool canBeHovered: 1;
        bool fixedWidth: 1;
        bool fixedHeight: 1;
        bool draggable: 1;
        bool needsDeletion: 1;
        bool wantGrowHorizontal: 1;
        bool wantGrowVertical: 1;
        bool hasTexture: 1;
    } flags;

    struct {
        Vec2i pos;
        Vec2i worldPos;

        int width;
        int height;
        int maxWidth;
        int maxHeight;

        int worldWidth;
        int worldHeight;

        int cornerRadius;

        float flexGrow;
    } dims;

    struct {
        Texture texture;

        Vec3f color;
        Vec3f defaultColor;

        float transparency;
        float brightness;
    } visuals;

    Cache layoutCache;

    ElementHandle handle;

    PositionMode positionMode;
    LayoutDirection layoutDirection;

    void (*generateMesh)(const Element* element, GuiVertex *vertices, int *vt, int *indices, int *id);

    struct {
        bool (*isMouseOver)(const Element* element, Vec2i mousePos);
        bool (*onClick)(Element* element, Renderer *renderer);
        bool (*onHover)(Element* element, Renderer *renderer);
        void (*whileSelected)(Element* element);
        void (*onUpdate)(Element* element);
        void (*reset)(Element* element);
        void (*requestMove)(Element* element, Vec2i pos);
    } callbacks;

    Padding padding;
    TextElement textElement;

    Task task;
    ElementHandle parentElement;
    ElementHandle* aFlowElements;
    ElementHandle* aStaticElements;
    int childGap;

    void* elementData;

} Element;


typedef struct ElementSettings {
    char* name;
    Vec2i pos;
    PositionMode posMode;
    LayoutDirection layoutDirection;

    int cornerRadius;
    int minWidth;
    int minHeight;
    int maxWidth;
    int maxHeight;
    float flexGrow;

    char* texture;
    Vec3f color;
    float transparency;
    char* text;
    bool (*onClick)(Element* element, Renderer *renderer);
    bool (*onHover)(Element* element, Renderer *renderer);
    void (*whileSelected)(Element* element);
    void (*onUpdate)(Element* element);
    void (*reset)(Element* element);

    Task task;
    Padding padding;
    int childGap;
    bool canBeHovered;
    bool autoFit;
    bool invisible;
    bool cantBeSelected;
    bool draggable;
    bool wantGrowHorizontal;
    bool wantGrowVertical;

    void* elementData;

} ElementSettings;

ElementHandle Element_addChildElements(ElementHandle parent, ...);

ElementHandle Element_addChildElements_vaList(ElementHandle parentHandle, va_list args);

Element* Element_get(ElementHandle handle);

ElementHandle addChildrenAsGrid(ElementSettings parentData, ElementSettings es, int numX, int numY);
ElementHandle addChildrenAsGridWithGenerator(ElementSettings parentData, ElementSettings es, int numX, int numY, ElementHandle (*generateElement)(int row, int col, ElementSettings));

void Element_setOnClickCallback(Element* element, bool (*onClick)(Element* element, Renderer* renderer));
void Element_setOnHoverCallback(Element* element, bool (*onHover)(Element* element, Renderer* renderer));
void Element_setBoundingBox(Element* element, bool (*isMouseOver)(const Element *element, Vec2i mousePos));

Element *Element_getElement_ptr(const char *name);
void Element_setText_ptr(Element* element, const char* text);
void Element_setText_int(Element* element, int i);
void Element_setActive_ptr(Element* element, bool b);
void Element_toggleVisible_ptr(Element* element);
void Element_setColor_ptr(Element* element, Vec3f color);

void Element_printDebug(const Element* element);

bool Element_isQuadBB(const Element *element, Vec2i mousePos);

ElementHandle Element_addElement(
    char *name,
    Vec2i pos,
    int width,
    int height,
    Vec3f color,
    Padding padding,
    int childGap,
    bool (*mouseOver)(const Element *, Vec2i),
    bool (*hover)(Element *, Renderer *),
    bool (*click)(Element *, Renderer *),
    Task task,
    const char *text,
    bool forceResize,
    PositionMode positionMode,
    void *elementData,
    bool notSelectable,
    LayoutDirection layoutDirection,
    int maxWidth,
    int maxHeight,
    void (*whileSelected)(Element *element), bool draggable, void (*onUpdate)(Element *element),
    bool wantGrowHorizontal, bool
    wantGrowVertical, float transparency, const char *texture, bool invisible, int cornerRadius, float flexGrow, bool
    canBeHovered
);

ElementHandle createElement(ElementSettings es);

ElementHandle _Element_new(ElementSettings es, ...);

#define Element_new(settings, ...) _Element_new(settings, __VA_ARGS__, (ElementHandle){-1})
#define addChildElements(parent, ...) Element_addChildElements(parent, __VA_ARGS__, (ElementHandle){-1})