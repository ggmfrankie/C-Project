//
// Created by Stefan on 18.11.2025.
//

#ifndef C_TEXTELEMENT_H
#define C_TEXTELEMENT_H
#include "Texture.h"
#include "stb/stb_truetype.h"
struct Element;
typedef struct Element Element;
struct Renderer;
typedef struct Renderer Renderer;

typedef struct Font{
    Texture fontAtlas;
    stbtt_packedchar glyphs[96];
    float fontSize;
} Font;

Font loadFontAtlas(String file);
void renderText(Renderer *renderer, const Element *element);

#endif //C_TEXTELEMENT_H