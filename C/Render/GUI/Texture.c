//
// Created by Stefan on 31.10.2025.
//

#include "../GUI/Texture.h"
#include "../../Utils/_Deprecated_/CString_v1.h"
#define STB_IMAGE_IMPLEMENTATION

#include <stb/stb_image.h>
#include <stb/stb_rect_pack.h>

#include "DataStructures/CArrayList.h"
#include "DataStructures/CString.h"
#include "glad/gl.h"
#include "Utils/DataStructures/CHashMap.h"
#include "Utils/Logging/Logging.h"
#include "Utils/Makros/Defer.h"

static constexpr int MAX_ATLAS_TEXTURES = 512;
static constexpr int PIXEL_SIZE = 4;

typedef struct {
    StandaloneTexture m[256];
    size_t capacity;
    size_t size;
} TextureList;

static GLuint uploadTextureToGPU(int width, int height, int channels, const unsigned char* pixels);

static Texture* g_map_textureMap;
static TextureList g_Textures = {.capacity = 256, .size = 0};

static StandaloneTexture* newTexture(const int width, const int height, const GLuint textureId) {
    assert(g_Textures.size < g_Textures.capacity);
    g_Textures.m[g_Textures.size] = (StandaloneTexture){.width = width, .height = height, .ID = textureId};
    return &g_Textures.m[g_Textures.size++];
}

void Texture_loadAtlas(TextureAtlas *atlas) {
    stbrp_rect rects[MAX_ATLAS_TEXTURES];
    byte* pixels[MAX_ATLAS_TEXTURES];
    const char* names[MAX_ATLAS_TEXTURES];
    int index = 0;

    const char* defaultPath = "../Resources/Textures/";

    constexpr int padding = 2;
    for_eachArr(namePtr, atlas->aNames, {
        int width, height, channels;
        const char* name = *namePtr;
        printf("Name: %s\n", name);

        char fullPath[64];
        cstrbConcat(fullPath, 64, defaultPath, name);

        pixels[index] = stbi_load(fullPath, &width, &height, &channels, 4);
        if (!pixels[index]) ERROR_("Error loading texture for Atlas");

        rects[index].w = width + padding*2;
        rects[index].h = height + padding*2;
        rects[index].id = index;
        names[index] = name;

        index++;
    });

    const int width = atlas->width;
    const int height = atlas->height;

    defer(defer_free) byte* data = calloc(atlas->width * atlas->height, PIXEL_SIZE);

    stbrp_context ctx;
    defer(defer_free) stbrp_node* nodes = malloc(sizeof(stbrp_node) * width);

    stbrp_init_target(&ctx, width, height, nodes, width);
    stbrp_pack_rects(&ctx, rects, index);

    for (int i = 0; i < index; i++) {
        if (!rects[i].was_packed) {
            WARNING_("Atlas pack failed for rect %d (%s)\n", i, names[i]);
            continue;
        }

        // where the texture should go. It is the rectangle with the padding, so we have to subtract that
        const int dstX = rects[i].x + padding;
        const int dstY = rects[i].y + padding;
        const int w    = rects[i].w - padding*2;
        const int h    = rects[i].h - padding*2;

        // the textures data
        const byte* src = pixels[i];

        for (int row = 0; row < h; ++row) {
            byte* dst = data + ((dstY + row) * atlas->width + dstX) * PIXEL_SIZE;
            const byte* s = src + row * (w * PIXEL_SIZE);
            memcpy(dst, s, w * PIXEL_SIZE);
        }

        mapInsert(g_map_textureMap, names[i], (Texture){
            .uv0 = {
                ((float)rects[i].x + padding)/(float)atlas->width,
                ((float)rects[i].y + padding)/(float)atlas->height
            },
            .uv1 = {
                (float)(rects[i].x - padding + w)/(float)atlas->width,
                (float)(rects[i].y - padding + h)/(float)atlas->height
            }
        });
    }
    atlas->ID = uploadTextureToGPU(atlas->width, atlas->height, PIXEL_SIZE, data);

    for (int i = 0; i < index; i++) {
        stbi_image_free(pixels[i]);
    }
}

void f_addTextures(TextureAtlas *atlas, const char *first, va_list args) {
    const char* file = first;

    while (file) {
        arrPush(atlas->aNames, file);
        file = va_arg(args, const char*);
    }
}

StandaloneTexture *Texture_new(const int width, const int height) {
    GLuint ID;
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA32F,
        width,
        height,
        0,
        GL_RGBA,
        GL_FLOAT,
        nullptr
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    return newTexture(width, height, ID);
}

StandaloneTexture *Texture_newFromPng(char *fileName) {
    const String fileNameString = stringOf(fileName);
    const String defaultPath = stringOf("../Resources/Textures/");
    defer(str_delete) String fullPath = Strings.combine(&defaultPath, &fileNameString);

    int width, height, channels;

    byte* data = stbi_load(fullPath.m, &width, &height, &channels, 0);

    if (!data) {
        WARNING_("Failed to load image\n");
        return nullptr;
    }
    const GLuint ID = uploadTextureToGPU(width, height, channels, data);

    stbi_image_free(data);
    return newTexture(width, height, ID);
}

Texture getTexture(const char* name) {
    if (name == nullptr) {
        WARNING_("Error loading texture: no name provided");
        return (Texture){};
    }
    const Texture* texture = mapGet(g_map_textureMap, name);
    if (texture == nullptr) texture = mapGet(g_map_textureMap, "White.png");
    return *texture;
}

static GLuint uploadTextureToGPU(const int width, const int height, const int channels, const byte* pixels) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0,
                 (channels == 4) ? GL_RGBA : GL_RGB,
                 width, height, 0,
                 (channels == 4) ? GL_RGBA : GL_RGB,
                 GL_UNSIGNED_BYTE, pixels);

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    return texture;
}

TextureAtlas TextureAtlas_new(const int width, const int height) {
    return (TextureAtlas){
        .width = width,
        .height = height,
        .ID = 0
    };
}

