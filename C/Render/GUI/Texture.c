//
// Created by Stefan on 31.10.2025.
//

#include "../GUI/Texture.h"
#include "../../Utils/CString.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <stb/stb_rect_pack.h>

#define MAX_ATLAS_TEXTURES 512

typedef struct {
    int width, height;
    u_char *pixels;
} RawTexture;

ARRAY_LIST(Texture, Basic_Texture)

static GLuint uploadTextureToGPU(int width, int height, int channels, const unsigned char* pixels);

static Basic_Texture textureArray[256];
static Hashmap_AtlasTextures g_textureMap;
static auto g_Textures = (List_Texture){.content = textureArray, .capacity = 256, .size = 0};

Basic_Texture newTexture(const int width, const int height, const GLuint textureId) {
    return (Basic_Texture){
        .width = width,
        .height = height,
        .ID = textureId
    };
}

void f_loadTextures(TextureAtlas *atlas, const char *first, va_list args) {

    g_textureMap = newHashmap_AtlasTextures(64);
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
        if (!pixels[index]) puts("Error loading texture for Atlas");

        rects[index].w = width;
        rects[index].h = height;
        rects[index].id = index;
        names[index] = file;

        index++;

        Strings.delete_(&fileNameString);
        Strings.delete_(&fullPath);
        file = va_arg(args, const char*);
    }

    unsigned char* data = calloc(atlas->width * atlas->height * 4, 1);

    stbrp_context ctx;
    stbrp_node* nodes = malloc(sizeof(stbrp_node) * atlas->width);

    stbrp_init_target(&ctx, atlas->width, atlas->height, nodes, atlas->width);
    stbrp_pack_rects(&ctx, rects, index);

    for (int i = 0; i < index; i++) {
        if (!rects[i].was_packed) {
            fprintf(stderr, "Atlas pack failed for rect %d (%s)\n", i, names[i]);
            continue;
        }

        const int dstX = rects[i].x;
        const int dstY = rects[i].y;
        const int w    = rects[i].w;
        const int h    = rects[i].h;

        const unsigned char* src = pixels[i];

        for (int row = 0; row < h; ++row) {
            unsigned char* dst = data + ((dstY + row) * atlas->width + dstX) * 4;
            const unsigned char* s = src + row * (w * 4);
            memcpy(dst, s, (size_t)w * 4);
        }
    }

    atlas->ID = uploadTextureToGPU((int)atlas->width, (int)atlas->height, 4, data);

    for (int i = 0; i < index; i++) {
        Hashmap_AtlasTextures_add(&g_textureMap, names[i], (Texture){
            .uv0 = {(float)rects[i].x/(float)atlas->width, (float)rects[i].y/(float)atlas->height},
            .uv1 = {(float)(rects[i].x + rects[i].w)/(float)atlas->width, (float)(rects[i].y + rects[i].h)/(float)atlas->height}
        });
    }

    for (int i = 0; i < index; i++) {
        stbi_image_free(pixels[i]);
    }
    free(nodes);
    free(data);
}

Basic_Texture *newEmptyTexture(const int width, const int height) {
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
        nullptr
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);


    Texture_ListAdd(&g_Textures, (Basic_Texture){.width = width, .height = height, .ID = tex});

    return Texture_ListGetLast(&g_Textures);
}

Basic_Texture *loadTextureFromPng(char *fileName) {
    const String fileNameString = stringOf(fileName);
    const String defaultPath = stringOf("../Resources/Textures/");
    String fullPath = Strings.combine(&defaultPath, &fileNameString);

    int width, height, channels;

    unsigned char* data = stbi_load(fullPath.content, &width, &height, &channels, 0);

    Strings.delete_(&fullPath);
    if (!data) {
        printf("Failed to load image\n");
        exit(-3) ;
    }
    const GLuint texture = uploadTextureToGPU(width, height, channels, data);

    stbi_image_free(data);
    Texture_ListAdd(&g_Textures, (Basic_Texture){.width = width, .height = height, .ID = texture});

    return Texture_ListGetLast(&g_Textures);
}

Texture getTexture(const char* name) {
    if (name == NULL) {
        puts("Error loading texture: no name provided");
        return (Texture){};
    }
    const Texture* texture = Hashmap_AtlasTextures_get(&g_textureMap, name);
    return *texture;
}

static GLuint uploadTextureToGPU(const int width, const int height, const int channels, const unsigned char* pixels) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0,
                 (channels == 4) ? GL_RGBA : GL_RGB,
                 width, height, 0,
                 (channels == 4) ? GL_RGBA : GL_RGB,
                 GL_UNSIGNED_BYTE, pixels);

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return texture;
}

TextureAtlas loadTextureAtlas(const int width, const int height) {
    return (TextureAtlas){
        .width = width,
        .height = height,
        .ID = 0
    };
}

