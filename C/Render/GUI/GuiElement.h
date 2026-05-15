//
// Created by Stefan on 28.10.2025.
//
#pragma once
#include "GuiDefines.h"
#include "GuiTypes.h"
#include "Tasks.h"

#include "../../Utils/Deprecated/HashMap.h"
#include "../../Utils/Vector.h"
#include "Render/Drawing/TextDisplaying.h"
#include "Utils/Network.h"

typedef struct Padding {
    int left;
    int up;
    int right;
    int down;
} Padding;

typedef enum PositionMode: byte {
    POS_FIT = 0,
    POS_ABSOLUTE
} PositionMode;

typedef enum LayoutDirection: byte {
    L_down = 0,
    L_right
} LayoutDirection;

typedef enum ElementType: byte {
    t_defaultElement,
    t_slider,
    t_textField
} ElementType;

typedef struct {
    int minWidth;
    int minHeight;

    int maxWidth;
    int maxHeight;

    float flexGrowX;
    float flexGrowY;
} CachedDimensions;

typedef struct Element {
    char* name;
    ElementType type;

    struct {
        bool isActive: 1;
        bool autoFit: 1;
        bool invisible: 1;
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
        int prefWidth;
        int prefHeight;

        int worldWidth;
        int worldHeight;

        int cornerRadius;
    } dims;

    struct {
        Texture texture;

        Vec3f color;
        Vec3f defaultColor;

        float transparency;
        float brightness;
    } visuals;

    CachedDimensions cachedDims;

    int ID;

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
    } callbacks;

    Padding padding;
    TextElement textElement;

    Task task;
    Element* parentElement;
    Element** aChildElements;
    int childGap;

    void* elementData;

} Element;

typedef struct ElementSettings {
    char* name;
    Vec2i pos;
    PositionMode posMode;
    LayoutDirection layoutDirection;

    int cornerRadius;
    int width;
    int height;

    Texture* old_texture;
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
    bool autoFit;
    bool invisible;
    bool notSelectable;
    bool draggable;
    bool fixedWidth;
    bool fixedHeight;
    bool wantGrowHorizontal;
    bool wantGrowVertical;

    void* elementData;

} ElementSettings;

Element* Element_new(Vec2i pos, int width, int height);
Element* f_addChildElements(Element* parent, ...);

Element* addChildrenAsGrid(ElementSettings parentData, ElementSettings es, int numX, int numY);
Element* addChildrenAsGridWithGenerator(ElementSettings parentData, ElementSettings es, int numX, int numY, Element* (*generateElement)(int row, int col, ElementSettings));

void Element_setOnClickCallback(Element* element, bool (*onClick)(Element* element, Renderer* renderer));
void Element_setOnHoverCallback(Element* element, bool (*onHover)(Element* element, Renderer* renderer));
void Element_setBoundingBox(Element* element, bool (*isMouseOver)(const Element *element, Vec2i mousePos));
void defaultReset(Element* element);

Element* Element_getElement(const char* name);
void Element_setText(Element* element, const char* text);
void Element_setText_int(Element* element, int i);
void Element_setActive(Element* element, bool b);
void Element_toggleVisible(Element* element);
void Element_setColor(Element* element, Vec3f color);

void Element_printDebug(const Element* element);

bool Element_isQuadBB(const Element *element, Vec2i mousePos);

#define addChildElements(parent, ...) f_addChildElements(parent, __VA_ARGS__, NULL)

#define addChildElementsN(parent, count, ...) f_addChildElementsN(parent, count, __VA_ARGS__)
#define fitMode (Vec2i){-1, -1}

Element *Element_addElement(
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
    bool fixedWidth,
    bool fixedHeight,
    void (*whileSelected)(Element *element), bool draggable, void (*onUpdate)(Element *element), bool wantGrowHorizontal, bool
    wantGrowVertical, float transparency, const char *texture, bool invisible, int cornerRadius
);

Element *createElement(ElementSettings es);
Element *createTextFieldElement( ElementSettings elementSettings, bool (*onEnterCallback)(Element* element, Renderer *renderer));
