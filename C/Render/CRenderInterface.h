//
// Created by ertls on 17.02.2026.
//

#ifndef MIXEDPROJECT_CRENDERINTERFACE_H
#define MIXEDPROJECT_CRENDERINTERFACE_H

#ifdef __cplusplus
extern "C"{
#endif
typedef struct Shader Shader;
    Shader newShader(char* vertexShaderFile, char* fragmentShaderFile);




#ifdef __cplusplus
    }
#endif

#endif //MIXEDPROJECT_CRENDERINTERFACE_H