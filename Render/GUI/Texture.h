//
// Created by Stefan on 31.10.2025.
//

#ifndef C_TEXTURE_H
#define C_TEXTURE_H
#include "glad/gl.h"
#include "../../Utils/String.h"

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
} Texture;



Texture newTexture(int width, int height, GLuint textureId);

Texture *newEmptyTexture(int width, int height);

Texture *loadTextureFromPng(char *fileName);

#endif //C_TEXTURE_H