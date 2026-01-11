//
// Created by Stefan on 28.10.2025.
//

#ifndef C_GUIELEMENT_H
#define C_GUIELEMENT_H
#include <stdbool.h>

#include "Mesh.h"
#include "Tasks.h"
#include "../GUI/Texture.h"
#include "../../Utils/SimpleArray.h"
#include "../../Utils/HashMap.h"
#include "../../Utils/Vector.h"

struct Renderer;
typedef struct Renderer Renderer;
struct Element;
typedef struct Element Element;

typedef struct TextElement {
    bool forceResize;
    String text;
    Vec3f textColor;
    float textScale;
} TextElement;

typedef struct Padding {
    int left;
    int up;
    int right;
    int down;
} Padding;

typedef enum {
    S_default = 0,
    S_active = 1,
} ElementState;

typedef enum {
    POS_FIT = 0,
    POS_ABSOLUTE = 1,
} PositionMode;

typedef enum {
    L_down = 0,
    L_up = 1,
    L_left = 1,
    L_right = 1
} LayoutDirection;

ARRAY_LIST(ChildElements, Element*)

typedef struct Element {
    char* name;
    bool isActive;

    Vec2i pos;
    Vec2i worldPos;

    PositionMode positionMode;
    LayoutDirection layoutDirection;

    int width;
    int height;

    int actualWidth;
    int actualHeight;

    Mesh Mesh;
    Texture* texture;
    Vec3f color;
    Vec3f defaultColor;

    //Text display
    bool hasText;
    TextElement textElement;

    //Interaction
    ElementState state;
    bool (*isMouseOver)(const Element* element, Vec2i mousePos);
    bool (*onClick)(Element* element, Renderer *renderer);
    bool (*onHover)(Element* element, Renderer *renderer);
    void (*reset)(Element* element);

    Task task;
    Element* parentElement;
    List_ChildElements childElements;
    Padding padding;
    int childGap;
    bool autoFit;

    bool fixedWidth;
    bool fixedHeight;

    bool needsDeletion;

    void* elementData;

} Element;

ARRAY_LIST(Element, Element)

HASH_MAP(Element, char*, Element*)

typedef struct {
    char* name;
    Vec2i pos;
    PositionMode posMode;
    LayoutDirection layoutDirection;
    int width;
    int height;
    Texture* texture;
    Vec3f color;
    char* text;
    bool (*onClick)(Element* element, Renderer *renderer);
    bool (*onHover)(Element* element, Renderer *renderer);
    //NYI
    void (*reset)(Element* element);

    Task task;
    Padding padding;
    int childGap;
    bool autoFit;
    bool notSelectable;
    bool fixedWidth;
    bool fixedHeight;

    void* elementData;

} ElementSettings;

void initElements();

Element newElement(Mesh mesh, Vec2i pos, int width, int height, Texture* texture);
Element* f_addChildElements(Element* parent, ...);

Element* addChildrenAsGrid(ElementSettings parentData, ElementSettings es, int numX, int numY);
Element* addChildrenAsGridWithGenerator(ElementSettings parentData, ElementSettings es, int numX, int numY, Element* (*generateElement)(ElementSettings));

void setOnClickCallback(Element* element, bool (*onClick)(Element* element, Renderer* renderer));
void setOnHoverCallback(Element* element, bool (*onHover)(Element* element, Renderer* renderer));
void setBoundingBox(Element* element, bool (*isMouseOver)(const Element *element, Vec2i mousePos));
void defaultReset(Element* element);

void setText(Element* element, const char* text);
void setText_int(Element* element, int i);
void setVisible(Element* element, bool b);
Element* getElement(const char* name);

bool isSelected_Quad(const Element *element, Vec2i mousePos);

#define addChildElements(parent, ...) f_addChildElements(parent, __VA_ARGS__, NULL)

#define addChildElementsN(parent, count, ...) f_addChildElementsN(parent, count, __VA_ARGS__)
#define fitMode (Vec2i){-1, -1}

Element *guiAddElement(
    List_Element *list,
    char *name,
    Mesh mesh,
    Vec2i pos,
    int width,
    int height,
    Texture *tex,
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
    void *elementData, bool notSelectable, LayoutDirection layoutDirection, bool fixedWidth, bool fixedHeight
);

Element *guiAddSimpleRectangle_Texture(
    Vec2i pos,
    int width,
    int height,
    Texture *tex
);

Element *guiAddSimpleRectangle_Color(
    Vec2i pos,
    int width,
    int height,
    Vec3f color
);

Element *guiAddSimpleButton_Texture(
    Vec2i pos,
    int width,
    int height,
    Texture *tex,
    Task task,
    const char *text
);

Element *guiAddSimpleButton_Color(
    Vec2i pos,
    int width,
    int height,
    Vec3f color,
    Task task,
    const char *text
);

Element *createElement(ElementSettings elementSettings);

typedef struct Element_Functions {
 //TODO
} Element_Functions;
#endif //C_GUIELEMENT_H