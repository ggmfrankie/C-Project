//
// Created by Stefan on 31.10.2025.
//

#include "Texture.h"

Texture newTexture(const int width, const int height, const GLuint textureId) {
    return (Texture){
        .width = width,
        .height = height,
        .textureId = textureId
    };
}

Texture newEmptyTexture(const int width, const int height) {
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    // Allocate empty floating-point texture (RGBA32F is flexible)
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA32F,        // internal format (high precision)
        width,
        height,
        0,
        GL_RGBA,           // format
        GL_FLOAT,          // data type
        NULL               // no data = empty texture
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);
    return (Texture){
        .width = width,
        .height = height,
        .textureId = tex
    };
}