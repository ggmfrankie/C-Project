//
// Created by Stefan on 19.04.2026.
//

#pragma once
typedef struct Element Element;
typedef struct Font Font;
typedef struct Shader Shader;
typedef struct Basic_Texture StandaloneTexture;
typedef struct Texture Texture;
typedef struct GUI_Update GUI_Update;
typedef struct GuiVertex GuiVertex;
typedef struct GuiState GuiState;
typedef struct GLFWwindow GLFWwindow;

typedef unsigned int GLuint;

typedef void(*GLFWkeyfun)(GLFWwindow* window, int key, int scancode, int action, int mods);
typedef void(*GLFWcharfun)(GLFWwindow* window, unsigned int codepoint);
typedef void(*GLFWcursorposfun)(GLFWwindow* window, double xpos, double ypos);
typedef void(*GLFWframebuffersizefun)(GLFWwindow* window, int width, int height);