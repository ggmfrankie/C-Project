//
// Created by Stefan on 10.10.2025.
//

#ifndef C_SHADER_H
#define C_SHADER_H
#include "../Utils/Map.h"
#include "../Utils/HashMap.h"
#include "../Utils/String.h"
#include "../Utils/Vector.h"
#include "glad/gl.h"

HASH_MAP(Uniforms, char*, GLint)

typedef struct Shader {
        int programId;
        int vertexId;
        int fragmentId;
        Hashmap_Uniforms uniforms;

    } Shader;

typedef struct ShaderFunction {
    void (*bind)(const struct Shader*);
    void (*unbind)();
    void (*createUniform)(struct Shader *shader, const char* name);
    void (*setUniform_f)(const Shader *shader, const char* name, float value);
    void (*setUniform_Vec2)(const Shader *shader, const char* name, Vec2f value);
    void (*setUniform_i)(const Shader *shader, const char* name, int value);

    String (*readShaderFile)(const String *fileName);
    int (*createShader)(const GLchar** shaderSource, int shaderType, int programId);
} ShaderFunction;

Shader newShader();
void Shader_bindProgram(const Shader *shader);
void Shader_createUniform(Shader *shader, const char* name);
void Shader_unbindProgram();
void setUniform_f(const Shader *shader, const char* name, float value);
void setUniform_Vec2(const Shader *shader, const char* name, Vec2f value);
void setUniform_i(const Shader *shader, const char* name, int value);

String readShaderFile(const String *fileName);
int createShader(const GLchar** shaderSource, int shaderType, int programId);

static ShaderFunction Shaders = {
    .bind = Shader_bindProgram,
    .unbind = Shader_unbindProgram,
    .createUniform = Shader_createUniform,
    .createShader = createShader,
    .readShaderFile = readShaderFile,
    .setUniform_f = setUniform_f,
    .setUniform_i = setUniform_i,
    .setUniform_Vec2 = setUniform_Vec2
};
#endif //C_SHADER_H