//
// Created by Stefan on 18.11.2025.
//

#ifndef C_TEXTELEMENT_H
#define C_TEXTELEMENT_H

#include "../GUI/Texture.h"
#include "../../../Dependencies/include/stb/stb_truetype.h"
#include "../../Utils/Vector.h"
#include "RenderTypes.h"

struct Element;
typedef struct Element Element;
struct Renderer;
typedef struct Renderer Renderer;
struct TextElement;
typedef struct TextElement TextElement;

typedef struct {
    Basic_Texture fontAtlas;
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
void accumulateTextQuads(const Element *element, GuiVertex *vertices, int *vt, int *indices, int *id, const Font *font);
Vec2i measureText(const Font *font, const String *text);
Vec2i measureElementText(const Font *font, const TextElement* textElement);

#endif //C_TEXTELEMENT_H