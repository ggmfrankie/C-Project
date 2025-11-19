//
// Created by Stefan on 18.11.2025.
//

#include "Font.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>

#include "GuiElement.h"
#include "../Render.h"
#include "../../Utils/DataStructures.h"
#include "../../Utils/String.h"


Font loadFontAtlas(const String file) {
    const String defaultPath = stringOf("../Resources/Fonts/");
    const String completePath = Strings.combine(&defaultPath, &file);

    Strings.println(&completePath);

    unsigned char* ttf_buffer = malloc(1 << 20);
    unsigned char* temp_bitmap = malloc(512*512);

    FILE* f = fopen(completePath.content, "rb");
    size_t bytesRead = fread(ttf_buffer, 1, 1<<20, f);
    if (bytesRead == 0) {
        printf("Failed to read font file\n");
    }
    fclose(f);
    Strings.delete(&completePath);

    Font font;

    stbtt_BakeFontBitmap(ttf_buffer,0, 32.0f, temp_bitmap,512,512, 32,96,font.glyphs);
    font.fontSize = 32.0f;

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE, temp_bitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    font.fontAtlas = (Texture){
        .width = 512,
        .height = 512,
        .textureId = tex
    };

    free(temp_bitmap);
    free(ttf_buffer);
    return font;
}

void renderText(Renderer *renderer, const Element *element) {
    const TextElement *textElement = element->textElement;
    if (textElement->text.length == 0) return;
    Font* font = &renderer->font;
    float textScale = textElement->textScale;

    //TODO relative Position
    Vec2f startPos = (Vec2f){
        .x = element->pos.x + textElement->offset.x,
        .y = element->pos.y + element->height/2 + textElement->offset.y
    };
    Vec2f cursor = (Vec2f){
        .x = startPos.x,
        .y = startPos.y
    };

    glBindTexture(GL_TEXTURE_2D, font->fontAtlas.textureId);

    for (int i = 0; i < textElement->text.length; i++) {
        char c = textElement->text.content[i];

        if (c < 32 || c > 126) continue;

        stbtt_bakedchar* glyph = &font->glyphs[c - 32];

        float x0 = cursor.x + glyph->xoff * textElement->textScale;
        float y0 = cursor.y + glyph->yoff * textElement->textScale; // y inverted

        float s0 = glyph->x0 / (float) font->fontAtlas.width;
        float t0 = glyph->y0 / (float) font->fontAtlas.height;
        float s1 = glyph->x1 / (float) font->fontAtlas.width;
        float t1 = glyph->y1 / (float) font->fontAtlas.height;

        float glyphWidth = (glyph->x1 - glyph->x0) * textScale;
        float glyphHeight = (glyph->y1 - glyph->y0) * textScale;

        Shader* shader = &renderer->shader;
        setUniform_i(shader, stringOf("hasTexture"), 1);
        setUniform_f(shader, stringOf("width"), glyphWidth);
        setUniform_f(shader, stringOf("height"), glyphHeight);
        setUniform_Vec2(shader, stringOf("positionObject"), (Vec2f){.x = x0, .y = y0});

        setUniform_i(shader, stringOf("transformTexCoords"), 1);
        setUniform_Vec2(shader, stringOf("texPosStart"), (Vec2f){.x = s0, .y = t0});
        setUniform_Vec2(shader, stringOf("texPosEnd"), (Vec2f){.x = s1, .y = t1});
        cursor.x += glyph->xadvance * textElement->textScale;

        Mesh_render(&renderer->basicQuadMesh);
    }
    setUniform_i(&renderer->shader, stringOf("transformTexCoords"), 0);
}