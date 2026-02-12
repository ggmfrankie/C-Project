//
// Created by Stefan on 29.10.2025.
//

#include "ComputeShader.h"

#include <math.h>

#include "Shader.h"
#include "../Utils/String.h"
#include "../Utils/DataStructures.h"
#include "../Utils/Vector.h"

static GLuint generateGraphSSBO(const size_t size) {
    GLuint graphSSBO;
    glGenBuffers(1, &graphSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, graphSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr) (sizeof(Vec2f) * size), NULL, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, graphSSBO);
    return graphSSBO;
}

static GLuint createGraphingShader(const String *fileName, const int programId) {
    String shaderSource = readShaderFile(fileName);
    const GLchar* source = (GLchar*)shaderSource.content;

    const int shaderId = createShader(&source, GL_COMPUTE_SHADER, programId);

    int success;
    char infoLog[512];

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
        printf("Graphing Shader Compile Error:\n%s\n", infoLog);
    }

    Strings.delete(&shaderSource);
    return shaderId;
}

void ComputeShader_createUniform(ComputeShader *shader, const char* name) {
    const int uniformLocation = glGetUniformLocation(shader->programId, name);

    if(uniformLocation < 0){
        printf("Error creating Uniform");
    }
    Hashmap_Uniforms_add(&shader->uniforms, name, uniformLocation);
}

ComputeShader newComputeShader(Simple_Texture *texture, const int size) {
    const int programId = glCreateProgram();

    const String graphingShader = stringOf("GraphingShader.comp");

    const GLuint graphingId = createGraphingShader(&graphingShader, programId);
    glAttachShader(programId, graphingId);
    glLinkProgram(programId);

    int success;
    char infoLog[512];
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programId, 512, NULL, infoLog);
        printf("Shader Program Link Error:\n%s\n", infoLog);
    }
    glDeleteShader(graphingId);

    return (ComputeShader){
        .SSBO = generateGraphSSBO(size),
        .programId = programId,
        .texture = texture,
        .uniforms = newHashmap_Uniforms(16),
        .ssboSize = size,
        .thickness = 0.0f,
        .startX = 0.0f,
        .endX = 0.0f
    };
}

static void setBufferData_Vec2f(const ComputeShader *computeShader, const vec2_Array *data) {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, computeShader->SSBO);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, (long long) (data->size * sizeof(Vec2f)), data->content);
}

void ComputeShader_update(const ComputeShader *computeShader, double (*func)(double x)) {
    const int size = (int) computeShader->ssboSize;
    vec2_Array data = vec2_newArray(malloc(sizeof(Vec2f)* size), size);

    const float xStart = computeShader->startX;
    const float xEnd = computeShader->endX;

    double minVal = func(xStart);
    double maxVal = func(xStart);

    // first pass: find min/max over domain
    for (int i = 0; i < size; i++) {
        double x = xStart + (xEnd - xStart) * (float) i / (float)(size - 1);
        double y = func(x);
        if (y < minVal) minVal = y;
        if (y > maxVal) maxVal = y;
    }

    // second pass: normalize to 0..1
    for (int i = 0; i < size; i++) {
        double x = xStart + (xEnd - xStart) * (float) i / (float)(size - 1);
        double y = func(x);
        Vec2f normPos;
        normPos.x = (float)i / (float)(size - 1);
        normPos.y = (float)((y - minVal) / (maxVal - minVal));
        vec2_Array_set(&data, i, normPos);
    }

    setBufferData_Vec2f(computeShader, &data);
    vec2_Array_delete(&data);
}

void ComputeShader_run(const ComputeShader *computeShader) {
    const Simple_Texture *texture = computeShader->texture;
    glUseProgram(computeShader->programId);
    glBindImageTexture(
        0,                          // image unit index in shader (binding = 0)
        texture->textureId,         // texture ID you created (your "empty" graph texture)
        0,                          // mip level
        GL_FALSE,                   // not layered (2D texture)
        0,                          // layer index (ignored for 2D)
        GL_WRITE_ONLY,              // access type (the shader will write to it)
        GL_RGBA32F                  // texture format (must match shader's layout)
    );
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, computeShader->SSBO);
    glUniform1i(glGetUniformLocation(computeShader->programId, "dataSize"), (GLint)computeShader->ssboSize);
    glUniform1f(glGetUniformLocation(computeShader->programId, "thickness"), computeShader->thickness);
    glDispatchCompute((texture->width + 15)/16, (texture->height + 15)/16, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}