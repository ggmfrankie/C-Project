//
// Created by Stefan on 28.10.2025.
//
#pragma once
#include "GuiTypes.h"
#include "Tasks.h"

#include "../../Utils/HashMap.h"
#include "../../Utils/Vector.h"

typedef struct Padding {
    int left;
    int up;
    int right;
    int down;
} Padding;

typedef enum PositionMode {
    POS_FIT = 0,
    POS_ABSOLUTE = 1,
} PositionMode;

typedef enum LayoutDirection {
    L_down,
    L_right
} LayoutDirection;

typedef enum ElementType {
    t_defaultElement,
    t_slider,
    t_textField
} ElementType;

ARRAY_LIST(ChildElements, Element*)

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
        float transparency;
        float brightness;
        Texture texture;
        Vec3f color;
        Vec3f defaultColor;
    } visuals;

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
    List_ChildElements childElements;
    int childGap;

    void* elementData;

} Element;

ARRAY_LIST(Element, Element)

HASH_MAP(Element, char*, Element*)

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

void initElements();

Element* newElement(Vec2i pos, int width, int height);
Element* f_addChildElements(Element* parent, ...);

Element* addChildrenAsGrid(ElementSettings parentData, ElementSettings es, int numX, int numY);
Element* addChildrenAsGridWithGenerator(ElementSettings parentData, ElementSettings es, int numX, int numY, Element* (*generateElement)(int row, int col, ElementSettings));

void setOnClickCallback(Element* element, bool (*onClick)(Element* element, Renderer* renderer));
void setOnHoverCallback(Element* element, bool (*onHover)(Element* element, Renderer* renderer));
void setBoundingBox(Element* element, bool (*isMouseOver)(const Element *element, Vec2i mousePos));
void defaultReset(Element* element);

void setText(Element* element, const char* text);
void setText_noLock(Element* element, const char* text);
void setText_int(Element* element, int i);
void setActive(Element* element, bool b);
void toggleVisible(Element* element);
void Element_setColor(Element* element, Vec3f color);
Element* getElement(const char* name);

bool isSelected_Quad(const Element *element, Vec2i mousePos);

#define addChildElements(parent, ...) f_addChildElements(parent, __VA_ARGS__, NULL)

#define addChildElementsN(parent, count, ...) f_addChildElementsN(parent, count, __VA_ARGS__)
#define fitMode (Vec2i){-1, -1}

Element *guiAddElement(
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
    wantGrowVertical, float transparency, char *texture, bool invisible, int cornerRadius
);

Element *createElement(ElementSettings es);
Element *createTextFieldElement( ElementSettings elementSettings, bool (*onEnterCallback)(Element* element, Renderer *renderer));
