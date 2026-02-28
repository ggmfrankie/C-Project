//
// Created by Stefan on 10.10.2025.
//


#ifndef C_RENDER_H
#define C_RENDER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ComputeShader.h"
#include "GUI/GuiElement.h"
#include "Shader.h"
#include "../../Dependencies/include/GLFW/glfw3.h"
#include "GUI/TextDisplaying.h"
#include "GUI/Texture.h"

extern Mesh quadMesh;

typedef struct Renderer {
    int screenWidth;
    int screenHeight;
    GLFWwindow *window;
    Vec2i mousePos;
    Shader guiShader;
    Shader batched_guiShader;
    Shader textShader;
    ComputeShader computeShader;

    Font font;
    GLuint atlasId;
    Hashmap_AtlasTextures textures;
    Mesh basicQuadMesh;
    bool (*defaultClick)(Renderer *renderer);
    void (*render)(const Renderer *renderer);

    Element* guiRoot;
    OtherShaders otherShaders;
} Renderer;

Renderer newRenderer(int width, int height, const char* name, char *fontFile);
Renderer newGUIRenderer(GLFWwindow* window, int width, int height, char *fontFile);
void GUIRenderer_init(Renderer *renderer);
void Renderer_render(const Renderer *renderer);
void GUI_render(const Renderer *renderer);
void Renderer_destroy(const Renderer *renderer);
Vec2i updateLayout(Element* element, Vec2i parentCursor, Vec2i parentPos, const Font* font);
bool isMousePressed(GLFWwindow* window, int mouseButton);
GLFWwindow* initWindow(int width, int height, const char* name);

#ifdef __cplusplus
} // extern "C"
#endif

#endif //C_RENDER_H