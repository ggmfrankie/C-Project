//
// Created by Stefan on 10.10.2025.
//


#ifndef C_RENDER_H
#define C_RENDER_H

#include "Shader.h"
#include "GLFW/glfw3.h"
#include "Mesh.h"


typedef struct Renderer {
    GLFWwindow *window;
    Shader shader;
    List_Mesh meshes;
} Renderer;

GLFWwindow* initWindow(const int width, const int height, const char* name);
void render(Renderer *renderer);
void destroyGL(const Renderer *renderer);

#endif //C_RENDER_H