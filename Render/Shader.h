//
// Created by Stefan on 10.10.2025.
//

#ifndef C_SHADER_H
#define C_SHADER_H
#include "../Utils/Map.h"
#include "../Utils/String.h"
#include "../Utils/Vector.h"
#include "glad/gl.h"

MAP(Uniforms, String, int)

typedef struct Shader {
        int programId;
        int vertexId;
        int fragmentId;
        Map_Uniforms uniforms;
        void (*bind)(const struct Shader*);
        void (*unbind)();
        void (*createUniform)(struct Shader *shader, String name);
    } Shader;

Shader newShader();
void Shader_bindProgram(const Shader *shader);
void Shader_createUniform(Shader *shader, String name);
void Shader_unbindProgram();
void setUniform_f(const Shader *shader, String name, float value);
void setUniform_Vec2(const Shader *shader, String name, Vec2f value);
void setUniform_i(const Shader *shader, String name, int value);

int createShader(const GLchar** shaderSource, int shaderType, int programId);
#endif //C_SHADER_H