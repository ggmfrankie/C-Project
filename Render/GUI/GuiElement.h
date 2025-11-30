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

typedef struct TextElement {
    bool forceResize;
    String text;
    Vec3f textColor;
    float textScale;
    Vec2f offset;
} TextElement;

typedef enum {
    S_default = 0,
    S_active = 1,
} ElementState;

typedef struct Element {
    Vec2f pos;
    float width;
    float height;
    short meshCount;
    Mesh Mesh;
    Texture* texture;

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

    int numChildElements;
    struct Element* childElements;
} Element;



SIMPLEARRAY(Element, Element)
ARRAY_LIST(Element, Element)

Element newElement(Mesh mesh, short meshCount, Vec2f pos, int width, int height, Texture* texture);
void setOnClickCallback(Element* element, bool (*onClick)(Element* element, Renderer* renderer));
void setOnHoverCallback(Element* element, bool (*onHover)(Element* element, Renderer* renderer));
void setBoundingBox(Element* element, bool (*isMouseOver)(const Element* element, Vec2f mousePos));
void setText(Element* element, const char* text);
void setText_int(Element* element, int i);
bool isSelected_Quad(const Element *element, Vec2f mousePos);
#endif //C_GUIELEMENT_H