//
// Created by Stefan on 31.10.2025.
//

#pragma once
#include <stdint.h>
#include <stdarg.h>

#include "GuiDefines.h"
#include "../../Utils/Math/Vector.h"

typedef struct NinePatchData {
    int stretchStartX;
    int stretchEndX;
    int stretchStartY;
    int stretchEndY;
} NinePatchData;

typedef struct Basic_Texture {
    int width;
    int height;
    uint32_t ID;

    NinePatchData npd;
} Basic_Texture;

typedef struct Texture {
    Vec2f uv0;
    Vec2f uv1;
} Texture;

typedef struct TextureAtlas {
    uint32_t ID;
    int width, height;
    Texture* map_textures;
} TextureAtlas;

Basic_Texture *newTexture(int width, int height, uint32_t textureId);

Basic_Texture *newEmptyTexture(int width, int height);

Basic_Texture *loadTextureFromPng(char *fileName);

TextureAtlas loadTextureAtlas(int width, int height);
void f_loadTextures(TextureAtlas *atlas, const char *first, va_list args);
Texture getTexture(const char* name);

#define loadTextures(a, w, h, ...) f_loadTextures(a, w, h, __VA_ARGS__, NULL)
