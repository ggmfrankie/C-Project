//
// Created by Stefan on 10.10.2025.
//


#ifndef C_RENDER_H
#define C_RENDER_H

#include "../ComputeShader.h"
#include "../GUI/GuiElement.h"
#include "../Shader.h"
#include "../../../Dependencies/include/GLFW/glfw3.h"
#include "TextDisplaying.h"
#include "../GUI/Texture.h"

typedef struct Renderer {
    int screenWidth;
    int screenHeight;
    GLFWwindow *window;
    Vec2i mousePos;

    Shader guiShader;

    ComputeShader computeShader;

    Font font;
    TextureAtlas texAtlas;

    Element* guiRoot;
} Renderer;

Renderer newGUIRenderer(GLFWwindow* window, int width, int height, char *fontFile);
void GUIRenderer_init(Renderer *renderer);

void Renderer_render2(const Renderer *renderer);

void Renderer_destroy(const Renderer *renderer);
Vec2i updateLayout(Element* element, Vec2i parentCursor, Vec2i parentPos, const Font* font);
bool isMousePressed(GLFWwindow* window, int mouseButton);
GLFWwindow* initWindow(int width, int height, const char* name);

#endif //C_RENDER_H