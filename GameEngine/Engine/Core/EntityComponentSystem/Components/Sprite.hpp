//
// Created by Stefan on 14.10.2026.
//

#pragma once
#include "glad/gl.h"

namespace ggm {
    struct Sprite {
        struct Texture {
            int width = 0;
            int height = 0;

            GLuint mTextureId = 0;
        } texture;

        struct Quad {
            GLuint VAO = 0;
            GLuint EBO = 0;
            GLuint vertexVBO = 0;
            GLuint textureVBO = 0;

            int indexCount = 0;
        } quad;

    };
}
