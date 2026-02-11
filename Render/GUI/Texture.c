//
// Created by Stefan on 31.10.2025.
//

#include "../GUI/Texture.h"
#include "../../Utils/String.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <stb/stb_rect_pack.h>
#include "../../Utils/HashMap.h"

#define MAX_ATLAS_TEXTURES 512

typedef struct {
    int width, height;
    u_char *pixels;
} RawTexture;

ARRAY_LIST(Texture, Simple_Texture)
HASH_MAP(AtlasTextures, char*, Texture)

Simple_Texture textureArray[256];
List_Texture g_Textures = (List_Texture){.content = textureArray, .capacity = 256, .size = 0};

Simple_Texture newTexture(const int width, const int height, const GLuint textureId) {
    return (Simple_Texture){
        .width = width,
        .height = height,
        .textureId = textureId
    };
}

Hashmap_AtlasTextures Texture_loadTextures(int atlasWidth, int atlasHeight, char *first, ...) {

    va_list args;
    va_start(args, first);

    Hashmap_AtlasTextures map = newHashmap_AtlasTextures(64);
    stbrp_rect rects[MAX_ATLAS_TEXTURES];
    u_char* pixels[MAX_ATLAS_TEXTURES];
    const char* names[MAX_ATLAS_TEXTURES];
    int index = 0;

    const char* file = first;

    while (file) {
        String fileNameString = newString_c(file);
        const String defaultPath = stringOf("../Resources/Textures/");
        String fullPath = Strings.combine(&defaultPath, &fileNameString);
        int width, height, channels;

        pixels[index] = stbi_load(fullPath.content, &width, &height, &channels, 4);
        rects[index].w = width;
        rects[index].h = height;
        rects[index].id = index;
        names[index] = file;
        index++;
        if (!pixels[index]) puts("Error loading texture for Atlas");

        Hashmap_AtlasTextures_add(&map, file, (Texture){});
        Strings.delete(&fileNameString);
        Strings.delete(&fullPath);
        file = va_arg(args, const char*);
    }
    va_end(args);

    unsigned char* atlas = calloc(atlasWidth * atlasHeight * 4, 1);

    stbrp_context ctx;
    stbrp_node* nodes = malloc(sizeof(stbrp_node) * atlasWidth);

    stbrp_init_target(&ctx, atlasWidth, atlasHeight, nodes, atlasWidth);
    stbrp_pack_rects(&ctx, rects, index);
}

Simple_Texture *newEmptyTexture(const int width, const int height) {
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA32F,
        width,
        height,
        0,
        GL_RGBA,
        GL_FLOAT,
        NULL
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);


    Texture_ListAdd(&g_Textures, (Simple_Texture){.width = width, .height = height, .textureId = tex});

    return Texture_ListGetLast(&g_Textures);
}

Simple_Texture *loadTextureFromPng(char *fileName) {
    const String fileNameString = stringOf(fileName);
    const String defaultPath = stringOf("../Resources/Textures/");
    String fullPath = Strings.combine(&defaultPath, &fileNameString);

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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    Texture_ListAdd(&g_Textures, (Simple_Texture){.width = width, .height = height, .textureId = texture});

    return Texture_ListGetLast(&g_Textures);
}

