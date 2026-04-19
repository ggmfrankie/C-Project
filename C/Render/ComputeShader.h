//
// Created by Stefan on 29.10.2025.
//
#pragma once

#include "Shader.h"
#include "GuiTypes.h"
#include "../../Dependencies/include/glad/gl.h"

typedef struct ComputeShader {
    GLuint SSBO;
    GLuint ssboSize;
    GLuint programId;
    Basic_Texture* texture;
    Hashmap_Uniforms uniforms;
    float thickness;
    float startX;
    float endX;
} ComputeShader;

ComputeShader newComputeShader(Basic_Texture *texture, int size);
void ComputeShader_update(const ComputeShader *computeShader, double (*func)(double x));
void ComputeShader_createUniform(ComputeShader *shader, const char* name);
void ComputeShader_run(const ComputeShader *computeShader);
