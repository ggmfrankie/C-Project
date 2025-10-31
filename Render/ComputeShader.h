//
// Created by Stefan on 29.10.2025.
//

#ifndef C_GRAPHINGSHADER_H
#define C_GRAPHINGSHADER_H
#include "Shader.h"
#include "Texture.h"
#include "glad/gl.h"

typedef struct ComputeShader {
    GLuint SSBO;
    int ssboSize;
    GLuint programId;
    Texture* texture;
    Map_Uniforms uniforms;
} ComputeShader;

ComputeShader newComputeShader(Texture *texture, int size);
void ComputeShader_update(const ComputeShader *computeShader, double (*func)(double x));
void ComputeShader_createUniform(ComputeShader *shader, String name);
void ComputeShader_run(const ComputeShader *computeShader);


#endif //C_GRAPHINGSHADER_H