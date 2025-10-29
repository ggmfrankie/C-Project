//
// Created by Stefan on 10.10.2025.
//


#ifndef C_RENDER_H
#define C_RENDER_H

#include "GuiElement.h"
#include "Shader.h"
#include "GLFW/glfw3.h"


typedef struct Renderer {
    int screenWidth;
    int screenHeight;
    GLFWwindow *window;
    Shader shader;
    Element_Array elements;
    void (*render)(struct Renderer *renderer);
} Renderer;

Renderer newRenderer(int width, int height, const char* name, Element_Array elements);
void Renderer_init(Renderer *renderer);
void Renderer_render(Renderer *renderer);
void Renderer_destroy(const Renderer *renderer);

#endif //C_RENDER_H