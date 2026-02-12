//
// Created by Stefan on 28.10.2025.
//

#ifndef C_GUIELEMENT_H
#define C_GUIELEMENT_H
#include <stdbool.h>


#include "Mesh.h"
#include "Tasks.h"
#include "TextDisplaying.h"
#include "../GUI/Texture.h"
#include "../../Utils/SimpleArray.h"
#include "../../Utils/HashMap.h"
#include "../../Utils/Vector.h"

struct Renderer;
typedef Renderer Renderer;
struct Element;
typedef Element Element;

typedef struct {
    Vec2f pos;
    Vec2f uv;

    Vec4f color;
    float brightness;

    bool hasTexture;

} GuiVertex;

typedef struct {
    GuiVertex vertices[32];
    int length;
    bool needUpdate;
} GuiMesh;

typedef struct TextElement {
    Vec2f pos;
    float width;
    bool hasText;
    bool forceResize;
    String text;
    Vec3f textColor;
    float textScale;

    List_Character charQuads;
} TextElement;

typedef struct Padding {
    int left;
    int up;
    int right;
    int down;
} Padding;

typedef enum {
    POS_FIT = 0,
    POS_ABSOLUTE = 1,
} PositionMode;

typedef enum {
    L_down,
    L_right
} LayoutDirection;

typedef enum {
    t_defaultElement,
    t_slider,
    t_textField
} ElementType;

struct Flags {
    bool isActive: 1;
    bool autoFit: 1;
    bool fixedWidth: 1;
    bool fixedHeight: 1;
    bool draggable: 1;
    bool needsDeletion: 1;
    bool wantGrowHorizontal: 1;
    bool wantGrowVertical: 1;
    bool hasTexture: 1;
};

typedef union {
    u_int64 data;
    struct Flags bits;
} ElementFlags;

ARRAY_LIST(ChildElements, Element*)

typedef struct Element {
    char* name;
    ElementType type;
    ElementFlags flags;

    Vec2i pos;
    Vec2i worldPos;
    float transparency;
    float brightness;

    PositionMode positionMode;
    LayoutDirection layoutDirection;

    int width;
    int height;

    int actualWidth;
    int actualHeight;

    Mesh Mesh;
    GuiMesh guiMesh;
    Simple_Texture* simpleTexture;
    Texture texture;
    Vec3f color;
    Vec3f defaultColor;

    //Text display
    TextElement textElement;

    //Interaction
    bool (*isMouseOver)(const Element* element, Vec2i mousePos);
    bool (*onClick)(Element* element, Renderer *renderer);
    bool (*onHover)(Element* element, Renderer *renderer);
    void (*whileSelected)(Element* element);
    void (*onUpdate)(Element* element);
    void (*reset)(Element* element);

    Task task;
    Element* parentElement;
    List_ChildElements childElements;
    Padding padding;
    int childGap;

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
    Simple_Texture* old_texture;
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
    bool notSelectable;
    bool draggable;
    bool fixedWidth;
    bool fixedHeight;
    bool wantGrowHorizontal;
    bool wantGrowVertical;

    void* elementData;

} ElementSettings;

void initElements();

Element newElement(Mesh mesh, Vec2i pos, int width, int height, Simple_Texture* texture);
Element* f_addChildElements(Element* parent, ...);

Element* addChildrenAsGrid(ElementSettings parentData, ElementSettings es, int numX, int numY);
Element* addChildrenAsGridWithGenerator(ElementSettings parentData, ElementSettings es, int numX, int numY, Element* (*generateElement)(ElementSettings));

void setOnClickCallback(Element* element, bool (*onClick)(Element* element, Renderer* renderer));
void setOnHoverCallback(Element* element, bool (*onHover)(Element* element, Renderer* renderer));
void setBoundingBox(Element* element, bool (*isMouseOver)(const Element *element, Vec2i mousePos));
void defaultReset(Element* element);

void setText(Element* element, const char* text);
void setText_noLock(Element* element, const char* text);
void setText_int(Element* element, int i);
void setVisible(Element* element, bool b);
void setColor(Element* element, Vec3f color);
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
    Simple_Texture *tex,
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
    wantGrowVertical, float transparency, char *texture
);

Element *guiAddSimpleRectangle_Texture(
    Vec2i pos,
    int width,
    int height,
    Simple_Texture *tex
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
    Simple_Texture *tex,
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

Element *createElement(ElementSettings es);
Element *createTextFieldElement( ElementSettings elementSettings);

typedef struct Element_Functions {
 //TODO
} Element_Functions;
#endif //C_GUIELEMENT_H