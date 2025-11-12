//
// Created by Stefan on 10.10.2025.
//


#ifndef C_RENDER_H
#define C_RENDER_H

#include "ComputeShader.h"
#include "GuiElement.h"
#include "Shader.h"
#include "GLFW/glfw3.h"


typedef struct Renderer {
    int screenWidth;
    int screenHeight;
    GLFWwindow *window;
    Vec2f mousePos;
    Shader shader;
    ComputeShader computeShader;
    List_Element elements;
    void (*render)(struct Renderer *renderer);
} Renderer;

Renderer newRenderer(int width, int height, const char* name, List_Element elements);
void Renderer_init(Renderer *renderer);
void Renderer_render(Renderer *renderer);
void Renderer_destroy(const Renderer *renderer);
bool isMousePressed(GLFWwindow* window, int mouseButton);

#endif //C_RENDER_H