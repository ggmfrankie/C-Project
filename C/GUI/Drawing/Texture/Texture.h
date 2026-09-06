//
// Created by Stefan on 31.10.2025.
//

#pragma once
#include <stdint.h>
#include <stdarg.h>

#include "GuiDefines.h"
#include "../../../Utils/Math/Vector.h"

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
} StandaloneTexture;

typedef struct Texture {
    Vec2f uv0;
    Vec2f uv1;
} Texture;

typedef struct TextureAtlas {
    uint32_t ID;
    int width, height;
    const char** aNames;
    Texture* map_textures;
} TextureAtlas;

void Texture_loadAtlas(TextureAtlas *atlas);

StandaloneTexture* Texture_new(int width, int height);

StandaloneTexture* Texture_newFromPng(const char *fileName);

TextureAtlas TextureAtlas_new(int width, int height);
void f_addTextures(TextureAtlas *atlas, const char *first, va_list args);
Texture Texture_get(const char* name);

#define loadTextures(a, w, h, ...) f_loadTextures(a, w, h, __VA_ARGS__, NULL)
