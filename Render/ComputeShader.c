//
// Created by Stefan on 29.10.2025.
//

#include "ComputeShader.h"
#include "Shader.h"
#include "../Utils/String.h"
#include "../Utils/FileIO.h"

GLuint generateGraphSSBO(const float *data, const size_t size) {
    GLuint graphSSBO;
    glGenBuffers(1, &graphSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, graphSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * size, data, GL_DYNAMIC_DRAW);
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

ComputeShader newComputeShader(const GLuint ssbo, const GLuint texture) {
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
        .graphSSBO = ssbo,
        .programId = programId,
        .texture = texture
    };
}