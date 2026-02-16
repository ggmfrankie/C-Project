//
// Created by Stefan on 31.10.2025.
//

#ifndef C_TEXTURE_H
#define C_TEXTURE_H
#include "glad/gl.h"
#include "../../Utils/CString.h"
#include "../../Utils/Vector.h"
#include "../../Utils/HashMap.h"

typedef struct {
    int stretchStartX;
    int stretchEndX;
    int stretchStartY;
    int stretchEndY;
} NinePatchData;

typedef struct {
    int width;
    int height;
    GLuint textureId;

    NinePatchData npd;
} Simple_Texture;

typedef struct {
    Vec2f uv0;
    Vec2f uv1;
} Texture;

HASH_MAP(AtlasTextures, char*, Texture);

Simple_Texture newTexture(int width, int height, GLuint textureId);

Simple_Texture *newEmptyTexture(int width, int height);

Simple_Texture *loadTextureFromPng(char *fileName);

void f_loadTextures(GLuint *atlasId, int atlasWidth, int atlasHeight, char *first, ...);
Texture getTexture(const char* name);

#define loadTextures(a, w, h, ...) f_loadTextures(a, w, h, __VA_ARGS__, NULL)

#endif //C_TEXTURE_H