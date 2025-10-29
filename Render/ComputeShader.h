//
// Created by Stefan on 29.10.2025.
//

#ifndef C_GRAPHINGSHADER_H
#define C_GRAPHINGSHADER_H
#include "glad/gl.h"

typedef struct ComputeShader {
    GLuint graphSSBO;
    GLuint programId;
    GLuint texture;
} ComputeShader;

#endif //C_GRAPHINGSHADER_H