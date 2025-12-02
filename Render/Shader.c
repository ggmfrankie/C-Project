//
// Created by Stefan on 10.10.2025.
//


#include "Shader.h"

#include <string.h>

#include "../Utils/FileIO.h"

ShaderFunction Shaders = {
    .bind = Shader_bindProgram,
    .unbind = Shader_unbindProgram,
    .createUniform = Shader_createUniform,
    .createShader = createShader,
    .readShaderFile = readShaderFile,
    .setUniform_f = setUniform_f,
    .setUniform_i = setUniform_i,
    .setUniform_Vec2 = setUniform_Vec2
};

int createVertexShader(const String *fileName, int programId);
int createFragmentShader(const String *fileName, int programId);


Shader newShader(char* vertexShaderFile, char* fragmentShaderFile) {

    const int programId = glCreateProgram();
    int success;
    char infoLog[512];
    const String vertexShader = stringOf(vertexShaderFile);
    const String fragmentShader = stringOf(fragmentShaderFile);

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
        .uniforms = newHashmap_Uniforms(16)
    };
}

void Shader_createUniform(Shader *shader, const char* name) {
    const int uniformLocation = glGetUniformLocation(shader->programId, name);

    if(uniformLocation < 0){
        printf("Error creating Uniform");
    }
    Hashmap_Name_add(&shader->uniforms, name, uniformLocation);
}

int createVertexShader(const String *fileName, const int programId) {
    String shaderSource = readShaderFile(fileName);
    const GLchar* source = (GLchar*)shaderSource.content;

    const int shaderId = createShader(&source, GL_VERTEX_SHADER, programId);
    Strings.delete(&shaderSource);

    return shaderId;
}

int createFragmentShader(const String *fileName, const int programId) {
    String shaderSource = readShaderFile(fileName);
    const GLchar* source = (GLchar*)shaderSource.content;

    const int shaderId = createShader(&source, GL_FRAGMENT_SHADER, programId);
    Strings.delete(&shaderSource);

    return shaderId;
}

String readShaderFile(const String *fileName) {
    const String defaultShaderPath = stringOf("../Shader/");
    String completePath = Strings.combine(&defaultShaderPath, fileName);
    const String shaderSource = readFile(&completePath);
    Strings.delete(&completePath);
    return shaderSource;
}

int createShader(const GLchar** shaderSource, const int shaderType, const int programId) {
    const int shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, shaderSource, NULL);
    glCompileShader(shaderId);
    glAttachShader(programId, shaderId);
    return shaderId;
}

void setUniform_f(const Shader *shader, const char* name, const float value) {
    glUniform1f(*Hashmap_Name_get(&shader->uniforms, name), value);
}

void setUniform_i(const Shader *shader, const char* name, const int value) {
    glUniform1i(*Hashmap_Name_get(&shader->uniforms, name), value);
}

void setUniform_Vec2(const Shader *shader, const char* name, const Vec2f value) {
    glUniform2f(*Hashmap_Name_get(&shader->uniforms, name), value.x, value.y);
}

void setUniform_Vec3(const Shader *shader, const char* name, const Vec3f value) {
    glUniform3f(*Hashmap_Name_get(&shader->uniforms, name), value.x, value.y, value.z);
}

void Shader_bindProgram(const Shader *shader) {
    glUseProgram(shader->programId);
}

void Shader_unbindProgram() {
    glUseProgram(0);
}


