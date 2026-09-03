//
// Created by Stefan on 10.10.2025.
//

#pragma once
#include "GuiDefines.h"
#include "Utils/Math/Vector.h"
#include "GuiTypes.h"
#include "Drawing/Shader/ComputeShader.h"
#include "GuiElement/GuiElement.h"

typedef struct GuiState {
    int screenWidth;
    int screenHeight;
    GLFWwindow *window;
    Vec2f mousePos;

    Shader guiShader;

    ComputeShader computeShader;

    Font font;
    TextureAtlas texAtlas;

    ElementHandle guiRoot;
} GuiState;

extern bool guiInitialized;

void startEngine(void (*generateGUI)(Element* guiRoot));
Vec2f getMousePos();
Vec2f getWindowSize();
Font* getFont();
double graphingFunction(double x);


