//
// Created by Stefan on 18.11.2025.
//

#pragma once
#include "GuiDefines.h"
#include <stdint.h>

#include "GuiTypes.h"

#include "../../../Dependencies/include/stb/stb_truetype.h"
#include "Utils/Math/Vector.h"
#include "Render/GUI/Texture.h"
#include "../../Utils/_Deprecated_/CString_v1.h"
#include "Render/GUI/ElementHandler.h"

typedef struct Font {
    StandaloneTexture fontAtlas;
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
    float scale;

    Character* aCharQuads;
} TextElement;

Font loadFontAtlas(const char* file);
void reloadTextQuads(const Font* font, Element *element);
void accumulateTextQuads(const Element *element, GuiVertex **aVertices, int **aIndices);
Vec2f measureElementText(const TextElement* textElement);
