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
    void (*render)(struct Renderer *renderer);

} Renderer;
Renderer newRenderer(int width, int height, const char* name);
void Renderer_render(Renderer *renderer);
void Renderer_destroy(const Renderer *renderer);

#endif //C_RENDER_H