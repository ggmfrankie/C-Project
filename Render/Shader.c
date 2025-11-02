//
// Created by Stefan on 10.10.2025.
//


#include "Shader.h"

#include <string.h>

#include "../Utils/FileIO.h"

int createVertexShader(const String *fileName, int programId);
int createFragmentShader(const String *fileName, int programId);

Shader newShader() {

    const int programId = glCreateProgram();
    int success;
    char infoLog[512];
    const String vertexShader = newString("GuiVertexShader.vert");
    const String fragmentShader = newString("GuiFragmentShader.frag");

    const int vertexId = createVertexShader(&vertexShader, programId);
    const int fragmentId = createFragmentShader(&fragmentShader, programId);

    glGetShaderiv(vertexId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexId, 512, NULL, infoLog);
        printf("Vertex Shader Compile Error:\n%s\n", infoLog);
    }

    // --- DEBUG: Check fragment shader compile status ---
    glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentId, 512, NULL, infoLog);
        printf("Fragment Shader Compile Error:\n%s\n", infoLog);
    }

    glLinkProgram(programId);

    // --- DEBUG: Check program link status ---
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programId, 512, NULL, infoLog);
        printf("Shader Program Link Error:\n%s\n", infoLog);
    } else {
        //printf("Shader Program linked successfully! ID: %d\n", programId);
    }

    return (Shader){
        .programId = programId,
        .vertexId = vertexId,
        .fragmentId = fragmentId,
        .bind = Shader_bindProgram,
        .unbind = Shader_unbindProgram,
        .createUniform = Shader_createUniform,
        .uniforms = newMap_Uniforms(16, str_equals)
    };
}

void Shader_createUniform(Shader *shader, const String name) {
    const int uniformLocation = glGetUniformLocation(shader->programId, name.content);

    if(uniformLocation < 0){
        printf("Error creating Uniform");
    }
    shader->uniforms.put(&shader->uniforms, newString_c(name.content), uniformLocation);
}

int createVertexShader(const String *fileName, const int programId) {
    const String shaderSource = readFile(fileName);
    const GLchar* source = (GLchar*)shaderSource.content;

    const int shaderId = createShader(&source, GL_VERTEX_SHADER, programId);
    shaderSource.delete(&shaderSource);

    return shaderId;
}

int createFragmentShader(const String *fileName, const int programId) {
    const String shaderSource = readFile(fileName);
    const GLchar* source = (GLchar*)shaderSource.content;

    const int shaderId = createShader(&source, GL_FRAGMENT_SHADER, programId);
    shaderSource.delete(&shaderSource);

    return shaderId;
}

int createShader(const GLchar** shaderSource, const int shaderType, const int programId) {
    const int shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, shaderSource, NULL);
    glCompileShader(shaderId);
    glAttachShader(programId, shaderId);
    return shaderId;
}

void Shader_bindProgram(const Shader *shader) {
    glUseProgram(shader->programId);
}

void Shader_unbindProgram() {
    glUseProgram(0);
}


