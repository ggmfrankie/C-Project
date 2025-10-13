//
// Created by Stefan on 10.10.2025.
//

#ifndef C_SHADER_H
#define C_SHADER_H
    typedef struct Shader {
        int programId;
        int vertexId;
        int fragmentId;
    } Shader;
Shader createShaderProgram();
void bindShaderProgram(int programId);
#endif //C_SHADER_H