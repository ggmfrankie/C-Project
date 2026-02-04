//
// Created by Stefan on 18.11.2025.
//

#include "TextDisplaying.h"

#define STB_RECT_PACK_IMPLEMENTATION
#include <stb/stb_rect_pack.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include <minwindef.h>
#include <stb/stb_truetype.h>
#include "GuiElement.h"
#include "../Render.h"
#include "../../Utils/DataStructures.h"
#include "../../Utils/String.h"
#define FONT_ATLAS_SIZE 2048
#define FONT_SIZE 32.0f

void measureFont(Font *font);

typedef struct {
    float x, y;     // position
    float u, v;     // UVs
} Vertex;


Font loadFontAtlas(char* file) {
    const String fileName = stringOf(file);
    const String defaultPath = stringOf("../Resources/Fonts/");
    String completePath = Strings.combine(&defaultPath, &fileName);

    Strings.println(&completePath);

    unsigned char* ttf_buffer = malloc(1 << 20);
    unsigned char* temp_bitmap = malloc(FONT_ATLAS_SIZE * FONT_ATLAS_SIZE);
    unsigned char* rgbaBuffer = malloc(FONT_ATLAS_SIZE * FONT_ATLAS_SIZE*4);

    FILE* f = fopen(completePath.content, "rb");
    const size_t bytesRead = fread(ttf_buffer, 1, 1<<20, f);
    if (bytesRead == 0) {
        printf("Failed to read font file\n");
    }
    fclose(f);
    Strings.delete(&completePath);

    Font font = {};

    font.fontSize = FONT_SIZE;

    stbtt_pack_context pc;
    stbtt_PackBegin(&pc, temp_bitmap, FONT_ATLAS_SIZE, FONT_ATLAS_SIZE, 0, 1, NULL);
    stbtt_PackSetOversampling(&pc, 3, 3);

    stbtt_PackFontRange(&pc, ttf_buffer, 0, FONT_SIZE,
                        32, 96, font.glyphs);
    stbtt_PackEnd(&pc);

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, FONT_ATLAS_SIZE, FONT_ATLAS_SIZE, 0, GL_ALPHA, GL_UNSIGNED_BYTE, temp_bitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    font.fontAtlas = (Texture){
        .width = FONT_ATLAS_SIZE,
        .height = FONT_ATLAS_SIZE,
        .textureId = tex
    };

    measureFont(&font);

    free(temp_bitmap);
    free(ttf_buffer);
    free(rgbaBuffer);
    return font;
}

void renderText(const Renderer *renderer, const Element *element) {
    const TextElement *textElement = &element->textElement;
    if (textElement->text.length == 0) return;

    const Font* font = &renderer->font;
    const float textScale = textElement->textScale;

    const Vec2i startPos = (Vec2i){
        .x = element->worldPos.x + element->padding.left,
        .y = element->worldPos.y + element->actualHeight - element->padding.down
    };
    Vec2f cursor = (Vec2f){
        .x = (float)startPos.x,
        .y = (float)startPos.y
    };

    glBindTexture(GL_TEXTURE_2D, font->fontAtlas.textureId);

    float maxGlyphHeight = 0;
    float totalGlyphLength = 0;
    for (int i = 0; i < textElement->text.length; i++) {
        const char c = textElement->text.content[i];
        if (c < 32 || c > 126) continue;

        stbtt_aligned_quad q;
        stbtt_GetPackedQuad(font->glyphs,
                            font->fontAtlas.width,
                            font->fontAtlas.height,
                            c - 32,
                            &cursor.x,
                            &cursor.y,
                            &q,
                            1);

        const float glyphWidth  = (q.x1 - q.x0) * textScale;
        const float glyphHeight = (q.y1 - q.y0) * textScale;

        totalGlyphLength += glyphWidth;
        if (maxGlyphHeight < glyphHeight) maxGlyphHeight = glyphHeight;

        const Shader* shader = &renderer->guiShader;
        setUniform_i(shader, ("hasTexture"), 1);
        setUniform_f(shader, ("width"), glyphWidth);
        setUniform_f(shader, ("height"), glyphHeight);
        setUniform_Vec2(shader, ("positionObject"), (Vec2f){ (q.x0-(float)startPos.x)*textScale + (float)startPos.x, (q.y0-(float)startPos.y)*textScale + (float)startPos.y });

        setUniform_i(shader, ("transformTexCoords"), 1);
        setUniform_Vec2(shader, ("texPosStart"), (Vec2f){ q.s0, q.t0 });
        setUniform_Vec2(shader, ("texPosEnd"), (Vec2f){ q.s1, q.t1 });

        Mesh_render(&renderer->basicQuadMesh);
    }
    setUniform_i(&renderer->guiShader, ("transformTexCoords"), 0);
}

Vec2i measureElementText(const Font *font, const TextElement* textElement) {
    const Vec2i measurements = measureText(font, &textElement->text);
    const float scale = textElement->textScale;
    return (Vec2i){(int)(measurements.x * scale), (int)(measurements.y * scale)};
}

Vec2i measureText(const Font *font, const String *text) {

    float x = 0.0f;
    float y = 0.0f;

    float maxHeight = 0.0f;

    for (int i = 0; i < text->length; i++) {
        const char c = text->content[i];
        if (c < 32 || c > 126) continue;

        stbtt_aligned_quad q;
        stbtt_GetPackedQuad(font->glyphs,
                            font->fontAtlas.width,
                            font->fontAtlas.height,
                            c - 32,
                            &x,
                            &y,
                            &q,
                            1);

        const float glyphHeight = (q.y1 - q.y0);

        maxHeight = max(maxHeight, glyphHeight);
    }

    return (Vec2i){
        (int)(x),
        (int)(maxHeight)
    };
}

void reloadTextQuads(const Font* font, Element *element) {
    TextElement *textElement = &element->textElement;
    if (textElement->text.length == 0) return;

    puts("in here?");
    Character_ListFree(&textElement->charQuads);

    const float textScale = textElement->textScale;

    const Vec2i startPos = (Vec2i){
        .x = element->worldPos.x + element->padding.left,
        .y = element->worldPos.y + element->actualHeight - element->padding.down
    };
    Vec2f cursor = (Vec2f){
        .x = (float)startPos.x,
        .y = (float)startPos.y
    };

    glBindTexture(GL_TEXTURE_2D, font->fontAtlas.textureId);

    float maxGlyphHeight = 0;
    float totalGlyphLength = 0;
    float prevX = 0.0f;

    for (int i = 0; i < textElement->text.length; i++) {
        const char c = textElement->text.content[i];
        if (c < 32 || c > 126) continue;
        Character_ListAdd(&textElement->charQuads, (Character){});
        Character* character = Character_ListGetLast(&textElement->charQuads);
        stbtt_aligned_quad q;
        stbtt_GetPackedQuad(font->glyphs,
                            font->fontAtlas.width,
                            font->fontAtlas.height,
                            c - 32,
                            &cursor.x,
                            &cursor.y,
                            &q,
                            1);

        const float glyphWidth  = (q.x1 - q.x0) * textScale;
        const float glyphHeight = (q.y1 - q.y0) * textScale;

        totalGlyphLength += glyphWidth;
        if (maxGlyphHeight < glyphHeight) maxGlyphHeight = glyphHeight;
        character->pos = (Vec2f){ (q.x0-(float)startPos.x)*textScale + (float)startPos.x, (q.y0-(float)startPos.y)*textScale + (float)startPos.y };
        character->width = glyphWidth;
        character->height = glyphHeight;
        character->texPosStart = (Vec2f){ q.s0, q.t0 };
        character->texPosEnd = (Vec2f){ q.s1, q.t1 };
        character->value = c;
        character->advance = cursor.x - prevX;
        prevX = cursor.x;
    }
}

[[deprecated]]
void renderTextOptimized(const Renderer *renderer, const Element *element) {
    const TextElement *textElement = &element->textElement;
    if (textElement->text.length == 0) return;

    Vertex vertices[textElement->text.length * 6];
    int vertCount = 0;

    const Font* font = &renderer->font;
    const float textScale = textElement->textScale;

    //TODO relative Position
    const Vec2f startPos = (Vec2f){
        .x = element->worldPos.x + element->padding.left,
        .y = element->worldPos.y + element->actualHeight - element->padding.down
    };
    Vec2f cursor = (Vec2f){
        .x = startPos.x,
        .y = startPos.y
    };

    glBindTexture(GL_TEXTURE_2D, font->fontAtlas.textureId);

    float maxGlyphHeight = 0;
    float totalGlyphLength = 0;
    for (int i = 0; i < textElement->text.length; i++) {
        const char c = textElement->text.content[i];
        if (c < 32 || c > 126) continue;

        stbtt_aligned_quad q;
        stbtt_GetPackedQuad(font->glyphs,
                            font->fontAtlas.width,
                            font->fontAtlas.height,
                            c - 32,
                            &cursor.x,
                            &cursor.y,
                            &q,
                            1);

        const float glyphWidth  = (q.x1 - q.x0) * textScale;
        const float glyphHeight = (q.y1 - q.y0) * textScale;

        totalGlyphLength += glyphWidth;
        if (maxGlyphHeight < glyphHeight) maxGlyphHeight = glyphHeight;
    }
}

void measureFont(Font *font) {
    const String allChars = stringOf("' !#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~'");
    const Vec2i fontSize = measureText(font, &allChars);
    font->maxCharHeight = fontSize.y;
}