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
    Vec2i offset;
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

ARRAY_LIST(ChildElements, Element*)

typedef struct Element {
    bool isActive;
    Vec2i pos;
    Vec2i worldPos;

    int width;
    int height;

    int actualWidth;
    int actualHeight;

    Mesh Mesh;
    Texture* texture;
    Vec3f color;

    //Text display
    bool hasText;
    TextElement textElement;

    //Interaction
    ElementState state;
    bool (*isMouseOver)(const struct Element* element, Vec2i mousePos);
    bool (*onClick)(struct Element* element, Renderer *renderer);
    bool (*onHover)(struct Element* element, Renderer *renderer);

    Task task;
    Element* parentElement;
    List_ChildElements childElements;
    Padding padding;
    int childGap;
    bool autoFit;

    bool needsDeletion;

    void* ElementData;

} Element;

ARRAY_LIST(Element, Element)

typedef struct {
    Vec2i pos;
    int width;
    int height;
    Texture* texture;
    Vec3f color;
    char* text;
    bool (*onClick)(Element* element, Renderer *renderer);
    bool (*onHover)(Element* element, Renderer *renderer);
    Task task;
    Padding padding;
    int childGap;
    bool autoFit;

} ElementSettings;


Element newElement(Mesh mesh, Vec2i pos, int width, int height, Texture* texture);
Element* f_addChildElements(Element* parent, ...);
Element* f_addChildElementsN(Element* parent, int count, ...);
void setOnClickCallback(Element* element, bool (*onClick)(Element* element, Renderer* renderer));
void setOnHoverCallback(Element* element, bool (*onHover)(Element* element, Renderer* renderer));
void setBoundingBox(Element* element, bool (*isMouseOver)(const Element *element, Vec2i mousePos));
void setText(Element* element, const char* text);
void setText_int(Element* element, int i);
bool isSelected_Quad(const Element *element, Vec2i mousePos);

#define addChildElements(parent, ...) f_addChildElements(parent, __VA_ARGS__, NULL)

#define addChildElementsN(parent, count, ...) f_addChildElementsN(parent, count, __VA_ARGS__)
#define fitMode (Vec2i){-1, -1}

Element *guiAddElement(
    List_Element *list,
    Mesh mesh,
    Vec2i pos,
    int width,
    int height,
    Texture *tex,
    Vec3f color,
    Padding padding,
    bool (*mouseOver)(const Element *, Vec2i),
    bool (*hover)(Element *, Renderer *),
    bool (*click)(Element *, Renderer *),
    Task task,
    const char *text,
    bool forceResize
);

Element *guiAddSimpleRectangle_Texture(
    List_Element *list,
    Vec2i pos,
    int width,
    int height,
    Texture *tex
);

Element *guiAddSimpleRectangle_Color(
    List_Element *list,
    Vec2i pos,
    int width,
    int height,
    Vec3f color
);

Element *guiAddSimpleButton_Texture(
    List_Element *list,
    Vec2i pos,
    int width,
    int height,
    Texture *tex,
    Task task,
    const char *text
);

Element *guiAddSimpleButton_Color(
    List_Element *list,
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