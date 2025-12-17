//
// Created by Stefan on 10.10.2025.
//


#ifndef C_RENDER_H
#define C_RENDER_H

#include "ComputeShader.h"
#include "GUI/GuiElement.h"
#include "Shader.h"
#include "GLFW/glfw3.h"
#include "GUI/TextDisplaying.h"

extern Mesh quadMesh;


typedef struct Renderer {
    int screenWidth;
    int screenHeight;
    GLFWwindow *window;
    Vec2i mousePos;
    Shader guiShader;
    ComputeShader computeShader;
    List_Element* elements;
    Font font;
    Mesh basicQuadMesh;
    bool (*defaultClick)(Renderer *renderer);
    void (*render)(Renderer *renderer);

    Element guiRoot;
    List_Element guiRoots;

    OtherShaders otherShaders;
} Renderer;

Renderer newRenderer(int width, int height, const char* name, List_Element* elements);
Renderer* newRenderer_h(int width, int height, const char* name, List_Element* elements);
void Renderer_init(Renderer *renderer);
void Renderer_render(Renderer *renderer);
void Renderer_destroy(const Renderer *renderer);
bool isMousePressed(GLFWwindow* window, int mouseButton);

#endif //C_RENDER_H