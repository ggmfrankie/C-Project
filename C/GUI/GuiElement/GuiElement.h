//
// Created by Stefan on 28.10.2025.
//
#pragma once
#include "GuiDefines.h"
#include "GuiTypes.h"
#include "IO/Tasks.h"

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

typedef enum ElementType: byte {
    t_defaultElement,
    t_slider,
    t_textField
} ElementType;

typedef enum UIState: byte {
    UI_STATE_NORMAL,
    UI_STATE_HOVER,
    UI_STATE_SELECTED,
    UI_STATE_DISABLED
} UIState;

typedef struct {
    int start;
    int end;
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
    char* name;
    ElementType type;
    UIState state;

    struct {
        bool isActive: 1;
        bool canBeHovered: 1;

        bool fixedWidth: 1;
        bool fixedHeight: 1;
        bool relativeWidth: 1;
        bool relativeHeight: 1;

        bool wantGrowHorizontal: 1;
        bool wantGrowVertical: 1;

        bool noLayoutContribution: 1;
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

        struct {
            Vec2f pos;
            Vec2f dims;
            bool hasClip;
        } clip;

    } visuals;

    Cache layoutCache;

    ElementHandle handle;

    PositionMode positionMode;
    LayoutDirection layoutDirection;

    void (*generateMesh)(const Element* element, GuiVertex** aVertices, int** aIndices, ssize_t id);

    struct {
        bool (*isMouseOver)(const Element* element, Vec2f mousePos);
        bool (*onClick)(Element* element);
        bool (*onHover)(Element* element);
        void (*whileSelected)(Element* element, double deltaTime);
        void (*onUpdate)(Element* element);
        void (*reset)(Element* element);
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
        bool needsFree;
    } elementData;

} Element;


typedef struct ElementSettings {
    char* name;
    Vec2f pos;
    PositionMode posMode;
    LayoutDirection layoutDirection;

    struct {
        Vec2f pos;
        Vec2f dims;
    } clipArea;

    float cornerRadius;
    float minWidth;
    float minHeight;
    float maxWidth;
    float maxHeight;
    float flexGrow;

    char* texture;
    Vec3f color;
    float transparency;

    char* text;
    float textScale;

    bool (*onClick)(Element* element);
    bool (*onHover)(Element* element);
    void (*whileSelected)(Element* element, double deltaTime);
    void (*onUpdate)(Element* element);
    void (*reset)(Element* element);
    void (*drawCustom)(const Element* element, GuiVertex** aVertices, int** aIndices, MeshInstanceData** additional, ssize_t ownerId);

    Task task;
    Padding padding;
    float childGap;
    bool canBeHovered;
    bool autoFit;
    bool invisible;
    bool canNotBeSelected;
    bool draggable;
    bool wantGrowHorizontal;
    bool wantGrowVertical;
    bool noLayoutContribution;

    void* elementData;

} ElementSettings;

void Element_init();
ElementHandle Element_addChildElements(Element *parent, ...);

ElementHandle Element_addChildElements_vaList(ElementHandle parentHandle, va_list args);

Element* Element_get(ElementHandle handle);
void Element_delete(ElementHandle handle);

ElementHandle addChildrenAsGrid(ElementSettings parentData, ElementSettings es, int numX, int numY);
ElementHandle addChildrenAsGridWithGenerator(ElementSettings parentData, ElementSettings es, int numX, int numY, ElementHandle (*generateElement)(int row, int col, ElementSettings));

void Element_setOnClickCallback(Element* element, bool (*onClick)(Element* element));
void Element_setOnHoverCallback(Element* element, bool (*onHover)(Element* element));
void Element_setBoundingBox(Element* element, bool (*isMouseOver)(const Element *element, Vec2f mousePos));

Element *Element_getElement_ptr(const char *name);
void Element_setText_ptr(Element* element, const char* text);
void Element_setText_int(Element* element, int i);
void Element_setActive_ptr(Element* element, bool b);
void Element_toggleVisible_ptr(Element* element);
void Element_setColor_ptr(Element* element, Vec3f color);

void Element_printDebug(const Element* element);


ElementHandle createElement(ElementSettings es);

ElementHandle _Element_new(ElementSettings es, ...);

#define Element_new(...) _Element_new(__VA_ARGS__, (ElementHandle){-1})
#define addChildElements(...) Element_addChildElements(__VA_ARGS__, (ElementHandle){-1})