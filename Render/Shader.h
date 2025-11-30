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

typedef struct OtherShaders {
    size_t length;
    Shader shaders[16];
} OtherShaders;

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

Shader newShader(char* vertexShaderFile, char* fragmentShaderFile);
void Shader_bindProgram(const Shader *shader);
void Shader_createUniform(Shader *shader, const char* name);
void Shader_unbindProgram();
void setUniform_f(const Shader *shader, const char* name, float value);
void setUniform_Vec2(const Shader *shader, const char* name, Vec2f value);
void setUniform_i(const Shader *shader, const char* name, int value);

#define setUniform(shader, name, value) _Generic((value),\
int: setUniform_i,\
float: setUniform_f,\
Vec2f: setUniform_Vec2\
)(shader, name, value)

String readShaderFile(const String *fileName);
int createShader(const GLchar** shaderSource, int shaderType, int programId);

extern ShaderFunction Shaders;
#endif //C_SHADER_H