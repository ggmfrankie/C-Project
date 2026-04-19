//
// Created by Stefan on 31.10.2025.
//

#pragma once

#include "../../../Dependencies/include/glad/gl.h"
#include "../../Utils/CString.h"
#include "../../Utils/Vector.h"
#include "../../Utils/HashMap.h"

typedef struct NinePatchData {
    int stretchStartX;
    int stretchEndX;
    int stretchStartY;
    int stretchEndY;
} NinePatchData;

typedef struct Basic_Texture {
    int width;
    int height;
    GLuint ID;

    NinePatchData npd;
} Basic_Texture;

typedef struct Texture {
    Vec2f uv0;
    Vec2f uv1;
} Texture;

HASH_MAP(AtlasTextures, char*, Texture);

typedef struct TextureAtlas {
    GLuint ID;
    GLuint width, height;
    Hashmap_AtlasTextures textures;
} TextureAtlas;

Basic_Texture newTexture(int width, int height, GLuint textureId);

Basic_Texture *newEmptyTexture(int width, int height);

Basic_Texture *loadTextureFromPng(char *fileName);

TextureAtlas loadTextureAtlas(int width, int height);
void f_loadTextures(TextureAtlas *atlas, const char *first, va_list args);
Texture getTexture(const char* name);

#define loadTextures(a, w, h, ...) f_loadTextures(a, w, h, __VA_ARGS__, NULL)
