//
// Created by Stefan on 31.10.2025.
//

#ifndef C_TEXTURE_H
#define C_TEXTURE_H
#include "glad/gl.h"
#include "../../Utils/String.h"

typedef struct {
    int width;
    int height;
    GLuint textureId;
} Texture;

Texture newTexture(int width, int height, GLuint textureId);
Texture newEmptyTexture(int width, int height);
Texture loadTextureFromPng(String fileName);

#endif //C_TEXTURE_H