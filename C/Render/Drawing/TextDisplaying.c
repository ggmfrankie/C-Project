//
// Created by Stefan on 18.11.2025.
//

#include "TextDisplaying.h"

#define STB_RECT_PACK_IMPLEMENTATION
#include <stb/stb_rect_pack.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include <float.h>
#include <stb/stb_truetype.h>
#include "Render.h"
#include "RenderTypes.h"
#include "Utils/CString_v1.h"
#include "Render/GUI/GuiElement.h"
#include "Utils/DataStructures/CArrayList.h"
#include "Utils/Makros/Defer.h"
#include "Utils/Makros/Makros.h"


#define FONT_ATLAS_SIZE 2048
#define FONT_SIZE 32.0f

static void measureFont(Font *font);

Font loadFontAtlas(const char* file) {
    const char* defaultPath = "../Resources/Fonts/";
    defer(defer_strDelete) CStr completePath = cstrConcat(defaultPath, file);

    defer(defer_free) byte* ttf_buffer = malloc(1 << 20);
    // ReSharper disable once CppDFAMemoryLeak
    defer(defer_free) byte* temp_bitmap = malloc(FONT_ATLAS_SIZE * FONT_ATLAS_SIZE);

    defer(defer_closeFile) FILE* f = fopen(completePath, "rb");

    assert(f != nullptr);
    const size_t bytesRead = fread(ttf_buffer, 1, 1<<20, f);
    if (bytesRead == 0) {
        printf("Failed to read font file\n");
        // ReSharper disable once CppDFAMemoryLeak
        return (Font){};
    }

    Font font = {};

    font.fontSize = FONT_SIZE;

    stbtt_pack_context pc;
    stbtt_PackBegin(&pc, temp_bitmap, FONT_ATLAS_SIZE, FONT_ATLAS_SIZE, 0, 1, nullptr);
    stbtt_PackSetOversampling(&pc, 3, 3);

    stbtt_PackFontRange(&pc, ttf_buffer, 0, FONT_SIZE,
                        32, 96, font.glyphs);
    stbtt_PackEnd(&pc);

    GLuint tex;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &tex);
    assert(tex != 0);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_R8,
        FONT_ATLAS_SIZE,
        FONT_ATLAS_SIZE,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        temp_bitmap
    );

    const GLint swizzle[] = { GL_ONE, GL_ONE, GL_ONE, GL_RED };
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    font.fontAtlas = (Basic_Texture){
        .width = FONT_ATLAS_SIZE,
        .height = FONT_ATLAS_SIZE,
        .ID = tex
    };

    measureFont(&font);
    return font;
}

void accumulateTextQuads(const Element *element, GuiVertex *vertices, int *vt, int *indices, int *id, const Font *font) {
    const Character* aCharQuads = element->textElement.aCharQuads;
    if (aCharQuads == nullptr || arrIsEmpty(aCharQuads)) return;

    const float xOffset = (float)element->padding.left;
    const float yOffset = (float)font->maxCharHeight * element->textElement.textScale + (float)element->padding.up;

    const int ID = element->ID;

    for_eachArr(c, aCharQuads, {
        const int texID = 1;

        const float x = c->pos.x + xOffset;
        const float y = c->pos.y + element->dims.worldHeight - element->padding.down;
        const float w = c->width;
        const float h = c->height;

        const Vec2f start = c->texPosStart;
        const Vec2f end   = c->texPosEnd;

        const Vec2f uv0 = start;
        const Vec2f uv1 = (Vec2f){ end.x, start.y };
        const Vec2f uv2 = end;
        const Vec2f uv3 = (Vec2f){ start.x, end.y };

        const int v0 = *vt;

        vertices[(*vt)++] = (GuiVertex){{x,   y},   uv0, ID, texID};
        vertices[(*vt)++] = (GuiVertex){{x+w, y},   uv1, ID, texID};
        vertices[(*vt)++] = (GuiVertex){{x+w, y+h}, uv2, ID, texID};
        vertices[(*vt)++] = (GuiVertex){{x,   y+h}, uv3, ID, texID};

        const int v1 = v0 + 1;
        const int v2 = v0 + 2;
        const int v3 = v0 + 3;

        indices[(*id)++] = v0; indices[(*id)++] = v1; indices[(*id)++] = v2;
        indices[(*id)++] = v0; indices[(*id)++] = v2; indices[(*id)++] = v3;
    });
}

Vec2i measureElementText(const TextElement* textElement) {

    if (arrIsEmpty(textElement->aCharQuads)) {
        return (Vec2i){0, 0};
    }

    float minX =  FLT_MAX;
    float minY =  FLT_MAX;
    float maxX = -FLT_MAX;
    float maxY = -FLT_MAX;

    for_eachArr(c, textElement->aCharQuads, {
        const float x0 = c->pos.x;
        const float y0 = c->pos.y;
        const float x1 = c->pos.x + c->width;
        const float y1 = c->pos.y + c->height;

        minX = min(minX, x0);
        minY = min(minY, y0);
        maxX = max(maxX, x1);
        maxY = max(maxY, y1);
    });

    const float width  = maxX - minX;
    const float height = maxY - minY;

    return (Vec2i){
        (int)width,
        (int)height
    };
}

Vec2i measureText(const Font *font, const String *text) {
    float x = 0.0f;
    float y = 0.0f;

    for (int i = 0; i < text->length; i++) {
        const char c = text->m[i];
        if (c < 32 || c > 126) continue;

        stbtt_aligned_quad q;
        stbtt_GetPackedQuad(
            font->glyphs,
            font->fontAtlas.width,
            font->fontAtlas.height,
            c - 32,
            &x,
            &y,
            &q,0
        );
    }

    return (Vec2i){
        (int)(x),
        (int)(font->maxCharHeight)
    };
}

void reloadTextQuads(const Font* font, Element *element) {
    TextElement *textElement = &element->textElement;
    arrClear(textElement->aCharQuads);

    if (textElement->text.length == 0) {
        textElement->width = 10;
        return;
    }

    const float textScale = textElement->textScale;

    const Vec2i startPos = {
        .x = 0,
        .y = 0
    };
    auto cursor = (Vec2f){
        .x = (float)startPos.x,
        .y = (float)startPos.y
    };

    glBindTexture(GL_TEXTURE_2D, font->fontAtlas.ID);

    float prevX = 0.0f;

    for (int i = 0; i < textElement->text.length; i++) {
        const char c = textElement->text.m[i];
        if (c < 32 || c > 126) continue;
        arrPush(textElement->aCharQuads, (Character){});
        Character* character = arrGetLast(textElement->aCharQuads);
        stbtt_aligned_quad q;
        stbtt_GetPackedQuad(font->glyphs,
                            font->fontAtlas.width,
                            font->fontAtlas.height,
                            c - 32,
                            &cursor.x,
                            &cursor.y,
                            &q,
                            0);

        const float glyphWidth  = (q.x1 - q.x0) * textScale;
        const float glyphHeight = (q.y1 - q.y0) * textScale;

        character->pos = (Vec2f){ (q.x0-(float)startPos.x)*textScale + (float)startPos.x, (q.y0-(float)startPos.y)*textScale + (float)startPos.y };
        character->width = glyphWidth;
        character->height = glyphHeight;
        character->texPosStart = (Vec2f){ q.s0, q.t0 };
        character->texPosEnd = (Vec2f){ q.s1, q.t1 };
        character->value = c;
        character->advance = cursor.x - prevX;
        prevX = cursor.x;
    }
    textElement->width = cursor.x * textScale;
}

void measureFont(Font *font) {
    const String allChars = stringOf("' !#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~'");
    const Vec2i fontSize = measureText(font, &allChars);
    font->maxCharHeight = fontSize.y;
}