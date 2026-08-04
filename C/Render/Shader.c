//
// Created by Stefan on 10.10.2025.
//


#include "Shader.h"
#include "Utils/Os/FileIO.h"
#include "Utils/DataStructures/CHashMap.h"
#include "Utils/Defer.h"
#include "Utils/Logging/Logging.h"

static int createVertexShader(const char *fileName, int programId);

static int createFragmentShader(const char *fileName, int programId);

Shader newShader(const char* vertexShaderFile, const char* fragmentShaderFile) {
    const int programId = glCreateProgram();
    int success;
    char infoLog[512];

    const int vertexId = createVertexShader(vertexShaderFile, programId);
    const int fragmentId = createFragmentShader(fragmentShaderFile, programId);

    glGetShaderiv(vertexId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexId, 512, nullptr, infoLog);
        ERROR("Vertex Shader Compile Error:\n%s\n", infoLog);
    }

    glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentId, 512, nullptr, infoLog);
        ERROR("Fragment Shader Compile Error:\n%s\n", infoLog);
    }

    glLinkProgram(programId);

    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programId, 512, nullptr, infoLog);
        ERROR("Shader Program Link Error:\n%s\n", infoLog);
    }

    return (Shader){
        .programId = programId,
        .vertexId = vertexId,
        .fragmentId = fragmentId,
        .mUniforms = nullptr
    };
}

void Shader_createUniform(Shader *shader, const char* name) {
    const int uniformLocation = glGetUniformLocation(shader->programId, name);

    if(uniformLocation < 0) ERROR("Error creating Uniform: %s", name);
    mapInsert(shader->mUniforms, name, uniformLocation);
}

int createVertexShader(const char *fileName, const int programId) {
    defer(defer_strDelete) CStr shaderSource = readShaderFile(fileName);
    const GLchar* source = shaderSource;

    const int shaderId = createShader(&source, GL_VERTEX_SHADER, programId);

    return shaderId;
}

int createFragmentShader(const char *fileName, const int programId) {
    defer(defer_strDelete) CStr shaderSource = readShaderFile(fileName);
    const GLchar* source = shaderSource;

    const int shaderId = createShader(&source, GL_FRAGMENT_SHADER, programId);

    return shaderId;
}

CStr readShaderFile(const char *fileName) {
    const char* defaultShaderPath = "../C/Shader/";
    defer(defer_strDelete) CStr completePath = cstrConcat(defaultShaderPath, fileName);
    CStr shaderSource = readFile(completePath);
    return shaderSource;
}

int createShader(const GLchar** shaderSource, const int shaderType, const int programId) {
    const int shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, shaderSource, nullptr);
    glCompileShader(shaderId);
    glAttachShader(programId, shaderId);
    return shaderId;
}

void setUniform_f(const Shader *shader, const char* name, const float value) {
    glUniform1f(*mapGet(shader->mUniforms, name), value);
}

void setUniform_i(const Shader *shader, const char* name, const int value) {
    glUniform1i(*mapGet(shader->mUniforms, name), value);
}

void setUniform_Vec2(const Shader *shader, const char* name, const Vec2f value) {
    glUniform2f(*mapGet(shader->mUniforms, name), value.x, value.y);
}

void setUniform_Vec3(const Shader *shader, const char* name, const Vec3f value) {
    glUniform3f(*mapGet(shader->mUniforms, name), value.x, value.y, value.z);
}

void Shader_bindProgram(const Shader *shader) {
    glUseProgram(shader->programId);
}

void Shader_unbindProgram() {
    glUseProgram(0);
}


