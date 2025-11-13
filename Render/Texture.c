//
// Created by Stefan on 31.10.2025.
//

#include "Texture.h"
#include "../Utils/String.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


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

Texture loadTextureFromPng(const String fileName) {
    const String defaultPath = stringOf("../Resources/Textures/");
    const String fullPath = Strings.combine(&defaultPath, &fileName);

    int width, height, channels;

    unsigned char* data = stbi_load(fullPath.content, &width, &height, &channels, 0);

    Strings.delete(&fullPath);
    if (!data) {
        printf("Failed to load image\n");
        exit(-3) ;
    }
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0,
                 (channels == 4) ? GL_RGBA : GL_RGB,
                 width, height, 0,
                 (channels == 4) ? GL_RGBA : GL_RGB,
                 GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    // Set filtering/wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Free CPU-side image memory
    stbi_image_free(data);
    return (Texture){
        .width = width,
        .height = height,
        .textureId = texture
    };
}