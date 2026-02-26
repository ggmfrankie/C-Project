//
// Created by Stefan on 29.10.2025.
//

#ifndef C_GRAPHINGSHADER_H
#define C_GRAPHINGSHADER_H
#include "Shader.h"
#include "GUI/Texture.h"
#include "../../Dependencies/include/glad/gl.h"

typedef struct ComputeShader {
    GLuint SSBO;
    GLuint ssboSize;
    GLuint programId;
    Texture* texture;
    Hashmap_Uniforms uniforms;
    float thickness;
    float startX;
    float endX;
} ComputeShader;

ComputeShader newComputeShader(Texture *texture, int size);
void ComputeShader_update(const ComputeShader *computeShader, double (*func)(double x));
void ComputeShader_createUniform(ComputeShader *shader, const char* name);
void ComputeShader_run(const ComputeShader *computeShader);


#endif //C_GRAPHINGSHADER_H