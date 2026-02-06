//
// Created by Stefan on 18.11.2025.
//

#ifndef C_TEXTELEMENT_H
#define C_TEXTELEMENT_H

#include "Texture.h"
#include "stb/stb_truetype.h"
#include "../../Utils/Vector.h"
struct Element;
typedef struct Element Element;
struct Renderer;
typedef struct Renderer Renderer;
struct TextElement;
typedef struct TextElement TextElement;

typedef struct {
    Texture fontAtlas;
    stbtt_packedchar glyphs[96];
    float fontSize;
    int maxCharHeight;

    GLuint textVAO;
    GLuint textVBO;
} Font;

typedef struct {
    Vec2f pos;
    float width;
    float height;
    float advance;

    Vec2f texPosStart;
    Vec2f texPosEnd;

    char value;
} Character;

ARRAY_LIST(Character, Character)

Font loadFontAtlas(char* file);
void reloadTextQuads(const Font* font, Element *element);
void renderText(const Renderer *renderer, const Element *element);
void renderTextRetained(const Renderer* renderer, const Element* element);
Vec2i measureText(const Font *font, const String *text);
Vec2i measureElementText(const Font *font, const TextElement* textElement);

#endif //C_TEXTELEMENT_H