//
// Created by Stefan on 31.10.2025.
//

#ifndef C_TEXTURE_H
#define C_TEXTURE_H
#include "glad/gl.h"
#include "../../Utils/String.h"
#include "../../Utils/Vector.h"

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
    GLuint atlasId;
    Vec2i uv0;
    Vec2i uv1;
} Texture;



Simple_Texture newTexture(int width, int height, GLuint textureId);

Simple_Texture *newEmptyTexture(int width, int height);

Simple_Texture *loadTextureFromPng(char *fileName);

#endif //C_TEXTURE_H