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
#include "../../Utils/CString.h"
#define FONT_ATLAS_SIZE 2048
#define FONT_SIZE 32.0f

#define MAX_TEXT_VERTICES 16384

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
    Strings.delete_(&completePath);

    Font font = {};

    font.fontSize = FONT_SIZE;

    stbtt_pack_context pc;
    stbtt_PackBegin(&pc, temp_bitmap, FONT_ATLAS_SIZE, FONT_ATLAS_SIZE, 0, 1, NULL);
    stbtt_PackSetOversampling(&pc, 3, 3);

    stbtt_PackFontRange(&pc, ttf_buffer, 0, FONT_SIZE,
                        32, 96, font.glyphs);
    stbtt_PackEnd(&pc);

    GLuint tex;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &tex);
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

    font.fontAtlas = (Texture){
        .width = FONT_ATLAS_SIZE,
        .height = FONT_ATLAS_SIZE,
        .textureId = tex
    };

    GLuint textVAO;
    GLuint textVBO;
    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);

    glBindVertexArray(textVAO);

    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glEnableVertexAttribArray(0); // position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void*)offsetof(Vertex, x));

    glEnableVertexAttribArray(1); // UV
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void*)offsetof(Vertex, u));

    glBufferData(GL_ARRAY_BUFFER,
             MAX_TEXT_VERTICES * sizeof(Vertex),
             NULL,
             GL_DYNAMIC_DRAW);

    font.textVAO = textVAO;
    font.textVBO = textVBO;

    measureFont(&font);

    free(temp_bitmap);
    free(ttf_buffer);
    free(rgbaBuffer);
    return font;
}

static void drawTextBatched(const Renderer* renderer, const Vertex* vertices, const int num) {
    glBindTexture(GL_TEXTURE_2D, renderer->font.fontAtlas.textureId);
    glBindVertexArray(renderer->font.textVAO);

    glBindBuffer(GL_ARRAY_BUFFER, renderer->font.textVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0,sizeof(Vertex) * num, vertices);

    glDrawArrays(GL_TRIANGLES, 0, num);
}

void renderTextRetained(const Renderer* renderer, const Element* element) {
    const TextElement *textElement = &element->textElement;
    if (textElement->text.length == 0) return;

    Shaders.bind(&renderer->textShader);

    Vertex vertices[textElement->charQuads.size * 6];

    Vec3f color = {};

    setUniform(&renderer->textShader, "fontAtlas", 0);
    setUniform(&renderer->textShader, ("screenWidth"), (float) renderer->screenWidth);
    setUniform(&renderer->textShader, ("screenHeight"), (float) renderer->screenHeight);
    setUniform(&renderer->textShader, "color", color);

    int v = 0;
    for (int i = 0; i < textElement->charQuads.size; i++) {
        const Character* c = &textElement->charQuads.content[i];

        const float x = c->pos.x + (float) element->worldPos.x + element->padding.left;
        const float y = c->pos.y + (float) element->worldPos.y + renderer->font.maxCharHeight * element->textElement.textScale + element->padding.up;
        const float w = c->width;
        const float h = c->height;

        const Vec2f uv0 = c->texPosStart;
        const Vec2f uv1 = c->texPosEnd;

        vertices[v++] = (Vertex){x, y, uv0.x, uv0.y};
        vertices[v++] = (Vertex){x+w, y, uv1.x, uv0.y};
        vertices[v++] = (Vertex){x+w, y+h, uv1.x, uv1.y};

        vertices[v++] = (Vertex){x, y, uv0.x, uv0.y};
        vertices[v++] = (Vertex){x+w, y+h, uv1.x, uv1.y};
        vertices[v++] = (Vertex){x, y+h, uv0.x, uv1.y};
    }

    drawTextBatched(renderer, vertices, v);
    Shaders.unbind();
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

    Character_ListClear(&textElement->charQuads);

    const float textScale = textElement->textScale;

    const Vec2i startPos = (Vec2i){
        .x = 0,
        .y = 0
    };
    Vec2f cursor = (Vec2f){
        .x = (float)startPos.x,
        .y = (float)startPos.y
    };

    glBindTexture(GL_TEXTURE_2D, font->fontAtlas.textureId);

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