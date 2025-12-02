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
    Vec2f offset;
} TextElement;

typedef struct Padding {
    float left;
    float up;
    float right;
    float down;
} Padding;

typedef enum {
    S_default = 0,
    S_active = 1,
} ElementState;


ARRAY_LIST(ChildElements, Element*)

typedef struct Element {
    Vec2f pos;
    Vec2f worldPos;

    float width;
    float height;

    float actualWidth;
    float actualHeight;

    short meshCount;
    Mesh Mesh;
    Texture* texture;
    Vec3f color;

    //Text display
    bool hasText;
    TextElement textElement;

    //Interaction
    ElementState state;
    bool isVisible;
    bool (*isMouseOver)(const struct Element* element, Vec2f mousePos);
    bool (*onClick)(struct Element* element, Renderer *renderer);
    bool (*onHover)(struct Element* element, Renderer *renderer);

    Task task;
    Element* parentElement;
    List_ChildElements childElements;
    Padding padding;
    bool autoFit;

} Element;

ARRAY_LIST(Element, Element)


Element newElement(Mesh mesh, short meshCount, Vec2f pos, int width, int height, Texture* texture);
Element* f_addChildElements(Element* parent, ...);
Element* f_addChildElementsN(Element* parent, int count, ...);
void setOnClickCallback(Element* element, bool (*onClick)(Element* element, Renderer* renderer));
void setOnHoverCallback(Element* element, bool (*onHover)(Element* element, Renderer* renderer));
void setBoundingBox(Element* element, bool (*isMouseOver)(const Element* element, Vec2f mousePos));
void setText(Element* element, const char* text);
void setText_int(Element* element, int i);
bool isSelected_Quad(const Element *element, Vec2f mousePos);

#define addChildElements(parent, ...) \
f_addChildElements(parent, __VA_ARGS__, NULL)

#define addChildElementsN(parent, count, ...) f_addChildElementsN(parent, count, __VA_ARGS__)


typedef struct Element_Functions {

}Element_Functions;
#endif //C_GUIELEMENT_H