//
// Created by Stefan on 28.10.2025.
//

#ifndef C_GUIELEMENT_H
#define C_GUIELEMENT_H
#include <stdbool.h>

#include "Mesh.h"
#include "../GUI/Texture.h"
#include "../../Utils/SimpleArray.h"
#include "../../Utils/Vector.h"

struct Renderer;
typedef struct Renderer Renderer;
struct TextElement;
typedef struct TextElement TextElement;

typedef enum {
    S_default = 0,
    S_active = 1,
} ElementState;

typedef struct Element {
    Vec2f pos;
    float width;
    float height;
    short meshCount;
    Mesh* Mesh;
    Texture* texture;
    //Text display

    TextElement* textElement;

    //Interaction
    ElementState state;
    bool isVisible;
    bool (*isMouseOver)(const struct Element* element, Vec2f mousePos);
    void (*onClick)(struct Element* element, Renderer *renderer);
    void (*onHover)(struct Element* element, Renderer *renderer);

    int numChildElements;
    struct Element* childElements;
} Element;

typedef struct TextElement {
    String text;
    Vec3f textColor;
    float textScale;
    Vec2f offset;
} TextElement;

SIMPLEARRAY(Element, Element)
ARRAY_LIST(Element, Element)

Element newElement(Mesh *mesh, short meshCount, Vec2f pos, int width, int height, Texture* texture);
void setOnClickCallback(Element* element, void (*onClick)(Element* element, Renderer* renderer));
void setOnHoverCallback(Element* element, void (*onHover)(Element* element, Renderer* renderer));
void setBoundingBox(Element* element, bool (*isMouseOver)(const Element* element, Vec2f mousePos));
bool isSelected_Quad(const Element *element, Vec2f mousePos);
#endif //C_GUIELEMENT_H