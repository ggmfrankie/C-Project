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

static Mesh quadMesh;

typedef struct Renderer {
    int screenWidth;
    int screenHeight;
    GLFWwindow *window;
    Vec2f mousePos;
    Shader guiShader;
    ComputeShader computeShader;
    List_Element* elements;
    Font font;
    Mesh basicQuadMesh;
    bool (*defaultClick)(Renderer *renderer);
    void (*render)(const Renderer *renderer);

    OtherShaders otherShaders;
} Renderer;

Renderer newRenderer(int width, int height, const char* name, List_Element* elements);
Renderer* newRenderer_h(int width, int height, const char* name, List_Element* elements);
void Renderer_init(Renderer *renderer);
void Renderer_render(const Renderer *renderer);
void Renderer_destroy(const Renderer *renderer);
void guiAddElement(
    List_Element* list,
    Mesh mesh,
    short meshCount,
    Vec2f pos,
    int width,
    int height,
    Texture* tex,
    bool (*mouseOver)(const Element*, Vec2f),
    bool (*hover)(Element*, Renderer*),
    bool (*click)(Element*, Renderer*),
    Task task,
    const char* text,
    bool forceResize
    );
void guiAddSimpleRectangle(
    List_Element* list,
    Vec2f pos,
    int width,
    int height,
    Texture* tex
    );
void guiAddSimpleButton(
    List_Element* list,
    Vec2f pos,
    int width,
    int height,
    Texture* tex,
    Task task,
    const char* text
    );
bool isMousePressed(GLFWwindow* window, int mouseButton);

#endif //C_RENDER_H