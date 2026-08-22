//
// Created by Stefan on 10.10.2025.
//

#pragma once
#include "GuiDefines.h"
#include "../ComputeShader.h"
#include "../Shader.h"
#include "TextDisplaying.h"
#include "Render/GUI/GuiElement.h"

typedef struct Renderer {
    int screenWidth;
    int screenHeight;
    GLFWwindow *window;
    Vec2f mousePos;

    Shader guiShader;

    ComputeShader computeShader;

    Font font;
    TextureAtlas texAtlas;

    ElementHandle guiRoot;
} Renderer;

Renderer Renderer_new(GLFWwindow* window, int width, int height, const char *fontFile);
void Renderer_init(Renderer *renderer);

void Renderer_render(const Renderer *renderer);
void Renderer_updateLayout(const Renderer *renderer);
void Renderer_destroy(const Renderer *renderer);

bool isMousePressed(GLFWwindow* window, int mouseButton);
GLFWwindow* initWindow(int width, int height, const char* name);

