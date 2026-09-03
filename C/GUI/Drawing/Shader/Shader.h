//
// Created by Stefan on 10.10.2025.
//
#pragma once
#include "GuiDefines.h"
#include "../Utils/_Deprecated_/CString_v1.h"
#include "../Utils/Math/Vector.h"
#include "../../Dependencies/include/glad/gl.h"
#include "Utils/DataStructures/CString.h"


typedef struct Shader {
    int programId;
    int vertexId;
    int fragmentId;
    GLint* mUniforms;
} Shader;

typedef struct OtherShaders {
    size_t length;
    Shader shaders[16];
} OtherShaders;

Shader newShader(const char* vertexShaderFile, const char* fragmentShaderFile);
void Shader_bindProgram(const Shader *shader);
void Shader_createUniform(Shader *shader, const char* name);
void Shader_unbindProgram();
void setUniform_f(const Shader *shader, const char* name, float value);
void setUniform_Vec2(const Shader *shader, const char* name, Vec2f value);
void setUniform_Vec3(const Shader *shader, const char* name, Vec3f value);
void setUniform_i(const Shader *shader, const char* name, int value);

#define setUniform(shader, name, value) _Generic((value),\
int: setUniform_i,\
float: setUniform_f,\
Vec2f: setUniform_Vec2,\
Vec3f: setUniform_Vec3\
)(shader, name, value)

CStr readShaderFile(const char *fileName);
int createShader(const GLchar** shaderSource, int shaderType, int programId);
