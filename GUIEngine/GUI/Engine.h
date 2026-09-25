//
// Created by Stefan on 10.10.2025.
//

#pragma once
#include "GuiDefines.h"
#include "Utils/Math/Vector.h"
#include "GuiTypes.h"
#include "Drawing/Shader/ComputeShader.h"
#include "Drawing/Shader/Shader.h"
#include "Drawing/Texture/Texture.h"
#include "GuiElement/GuiElement.h"

typedef struct GuiState {
    GLFWwindow *window;


    Shader guiShader;

    ComputeShader computeShader;

    Font font;
    TextureAtlas texAtlas;

    ElementHandle guiRoot;

    Vec2f mousePos;
    int screenWidth;
    int screenHeight;

    bool layoutDirty;
    bool meshesDirty;
} GuiState;

Vec2f getMousePos();
Vec2f getWindowSize();
Font* Engine_getDefaultFont();


