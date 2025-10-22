//
// Created by Stefan on 10.10.2025.
//

#ifndef C_SHADER_H
#define C_SHADER_H
#include "../Utils/Map.h"
#include "../Utils/String.h"

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
#endif //C_SHADER_H