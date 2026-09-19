//
// Created by Stefan on 10.10.2025.
//


#include "Shader.h"

#include <string.h>

#include "Utils/Os/FileIO.h"
#include "Utils/DataStructures/CHashMap.h"
#include "Utils/Macros/Defer.h"
#include "Utils/Logging/Logging.h"

static int createVertexShader(const char *fileName, int programId);

static int createFragmentShader(const char *fileName, int programId);

Shader Shader_new(const char* vertexShaderFile, const char* fragmentShaderFile) {
    Log_info("Loading shaders: '%s', '%s'", vertexShaderFile, fragmentShaderFile);
    const int programId = glCreateProgram();
    Log_debug("Program id: %i", programId);
    int success;
    char infoLog[512];

    const int vertexId = createVertexShader(vertexShaderFile, programId);
    const int fragmentId = createFragmentShader(fragmentShaderFile, programId);

    glGetShaderiv(vertexId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexId, 512, nullptr, infoLog);
        ERROR_("Vertex Shader Compile Error:\n%s\n", infoLog);
    }

    glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentId, 512, nullptr, infoLog);
        ERROR_("Fragment Shader Compile Error:\n%s\n", infoLog);
    }

    glLinkProgram(programId);
    glGetProgramiv(programId, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(programId, 512, nullptr, infoLog);
        ERROR_("Shader Program Link Error:\n%s\n", infoLog);
    }

    return (Shader){
        .programId = programId,
        .vertexId = vertexId,
        .fragmentId = fragmentId,
        .mUniforms = nullptr
    };
}

void Shader_createUniform(Shader *shader, const char* name) {
    Log_debug("Creating Uniform: '%s'", name);
    const int uniformLocation = glGetUniformLocation(shader->programId, name);

    if(uniformLocation < 0) ERROR_("Error creating Uniform: %s", name);
    mapInsert(shader->mUniforms, name, uniformLocation);
}

int createVertexShader(const char *fileName, const int programId) {
    Log_debug("Creating Vertex shader");
    defer(defer_strDelete) Str shaderSource = readShaderFile(fileName);
    const GLchar* source = shaderSource;

    const int shaderId = createShader(&source, GL_VERTEX_SHADER, programId);

    if (!shaderId) ERROR_("Failed to compile Vertex shader '%s'", fileName);

    return shaderId;
}

int createFragmentShader(const char *fileName, const int programId) {
    Log_debug("Creating Fragment shader");
    defer(defer_strDelete) Str shaderSource = readShaderFile(fileName);
    const GLchar* source = shaderSource;

    const int shaderId = createShader(&source, GL_FRAGMENT_SHADER, programId);

    if (!shaderId) ERROR_("Failed to compile Fragment shader '%s'", fileName);

    return shaderId;
}

Str readShaderFile(const char *fileName) {
    Log_debug("Reading shader file");
    const char* defaultShaderPath = GUI_PROJECT_SOURCE_DIR"/../GUIEngine/GUI/Drawing/Shader/GpuShader/";
    Log_debug("Default path %s", defaultShaderPath);

    char fullPath[strlen(defaultShaderPath) + strlen(fileName) + 1];
    cstrbConcat(fullPath, sizeof(fullPath), defaultShaderPath, fileName);
    Log_debug("Full path %s", fullPath);

    const Str shaderSource = readFile(fullPath);
    return shaderSource;
}

int createShader(const GLchar** shaderSource, const int shaderType, const int programId) {
    Log_debug("Creating shaders in OpenGL");
    const int shaderId = glCreateShader(shaderType);
    Log_debug("Compiling shader");
    glShaderSource(shaderId, 1, shaderSource, nullptr);
    glCompileShader(shaderId);
    Log_debug("Attaching Shader");
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


