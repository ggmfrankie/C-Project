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

    Font font;
    Mesh basicQuadMesh;
    bool (*defaultClick)(Renderer *renderer);
    void (*render)(const Renderer *renderer);

    Element guiRoot;

    OtherShaders otherShaders;
} Renderer;

Renderer newRenderer(int width, int height, const char* name);
void Renderer_init(Renderer *renderer);
void Renderer_render(const Renderer *renderer);
void Renderer_destroy(const Renderer *renderer);
Vec2i updateLayout(Element *element, Vec2i parentPos, const Renderer *renderer, int verticalOffset);
bool isMousePressed(GLFWwindow* window, int mouseButton);

#endif //C_RENDER_H