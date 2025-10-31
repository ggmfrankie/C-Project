//
// Created by Stefan on 29.10.2025.
//

#include "ComputeShader.h"

#include <math.h>

#include "Shader.h"
#include "../Utils/String.h"
#include "../Utils/FileIO.h"

GLuint generateGraphSSBO(const size_t size) {
    GLuint graphSSBO;
    glGenBuffers(1, &graphSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, graphSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * size, NULL, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, graphSSBO);
    return graphSSBO;
}

GLuint createGraphingShader(const String *fileName, const int programId) {
    const String shaderSource = readFile(fileName);
    const GLchar* source = (GLchar*)shaderSource.content;


    const int shaderId = createShader(&source, GL_COMPUTE_SHADER, programId);

    int success;
    char infoLog[512];

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
        printf("Graphing Shader Compile Error:\n%s\n", infoLog);
    }

    shaderSource.delete(&shaderSource);
    return shaderId;
}

void ComputeShader_createUniform(ComputeShader *shader, const String name) {
    const int uniformLocation = glGetUniformLocation(shader->programId, name.content);
    printf("Creating uniform '%s' -> location %d\n", name.content, uniformLocation);
    if(uniformLocation < 0){
        printf("Error creating Uniform");
    }
    shader->uniforms.put(&shader->uniforms, newString_c(name.content), uniformLocation);
}

ComputeShader newComputeShader(Texture *texture, const int size) {
    const int programId = glCreateProgram();

    const String graphingShader = newString("GraphingShader.comp");

    const GLuint graphingId = createGraphingShader(&graphingShader, programId);
    glAttachShader(programId, graphingId);
    glLinkProgram(programId);

    int success;
    char infoLog[512];
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programId, 512, NULL, infoLog);
        printf("Shader Program Link Error:\n%s\n", infoLog);
    } else {
        printf("Shader Program linked successfully! ID: %d\n", programId);
    }
    glDeleteShader(graphingId);

    return (ComputeShader){
        .SSBO = generateGraphSSBO(size),
        .programId = programId,
        .texture = texture,
        .uniforms = newMap_Uniforms(16, str_equals),
        .ssboSize = size
    };
}

void ComputeShader_update(const ComputeShader *computeShader, double (*func)(double x)) {
    const int size = computeShader->ssboSize;
    float* newData = malloc(sizeof(float)* size);
    float xStart = 0.0f;
    float xEnd = 100.0f;
    double minVal = func(xStart);
    double maxVal = func(xStart);

    // first pass: find min/max over domain
    for (int i = 0; i < size; i++) {
        double x = xStart + (xEnd - xStart) * i / (size - 1);
        double y = func(x);
        if (y < minVal) minVal = y;
        if (y > maxVal) maxVal = y;
    }

    // second pass: normalize to 0..1
    for (int i = 0; i < size; i++) {
        double x = xStart + (xEnd - xStart) * i / (size - 1);
        double y = func(x);
        newData[i] = (float)((y - minVal) / (maxVal - minVal));
    }
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, computeShader->SSBO);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(float) * size, newData);
    free(newData);
}

void ComputeShader_run(const ComputeShader *computeShader) {
    const Texture *texture = computeShader->texture;
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
    glUniform1i(glGetUniformLocation(computeShader->programId, "dataSize"), computeShader->ssboSize);
    glUniform1f(glGetUniformLocation(computeShader->programId, "thickness"), 100);
    glDispatchCompute((texture->width + 15)/16, (texture->height + 15)/16, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}