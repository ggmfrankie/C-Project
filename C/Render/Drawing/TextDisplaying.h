//
// Created by Stefan on 18.11.2025.
//

#pragma once
#include "GuiDefines.h"
#include <stdint.h>

#include "GuiTypes.h"

#include "../../../Dependencies/include/stb/stb_truetype.h"
#include "../../Utils/Vector.h"
#include "Render/GUI/Texture.h"
#include "Utils/CString.h"

typedef struct Font {
    Basic_Texture fontAtlas;
    stbtt_packedchar glyphs[96];
    float fontSize;
    int maxCharHeight;

    uint32_t textVAO;
    uint32_t textVBO;
} Font;

typedef struct Character {
    Vec2f pos;

    float width;
    float height;
    float advance;

    Vec2f texPosStart;
    Vec2f texPosEnd;

    char value;
} Character;

typedef struct TextElement {
    Vec2f pos;
    float width;
    bool hasText;
    bool forceResize;
    String text;

    Vec3f textColor;
    float textScale;

    Character* aCharQuads;
} TextElement;

Font loadFontAtlas(char* file);
void reloadTextQuads(const Font* font, Element *element);
void accumulateTextQuads(const Element *element, GuiVertex *vertices, int *vt, int *indices, int *id, const Font *font);
Vec2i measureText(const Font *font, const String *text);
Vec2i measureElementText(const TextElement* textElement);
