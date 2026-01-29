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

typedef struct {
    Texture fontAtlas;
    stbtt_packedchar glyphs[96];
    float fontSize;
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

#endif //C_TEXTELEMENT_H