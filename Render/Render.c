//
// Created by Stefan on 10.10.2025.
//
#include "glad/gl.h"
#include "Render.h"

#include <bemapiset.h>
#include <math.h>

#include "Engine.h"
#include "../Utils/Vector.h"
#include "GUI/CallbackFunctions.h"

Mesh quadMesh;

void loadDefaultQuadMesh();
void resizeCallback(GLFWwindow *window, int width, int height);
void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
void renderElementsRecursively(Element* element, const Renderer* renderer);

Vec2i measureText(const Renderer *renderer, const TextElement *textElement);

Vec2i updateLayout(Element *element, Vec2i parentPos, const Renderer *renderer, int verticalOffset);
Element createRootElement();

GLFWwindow* initWindow(const int width, const int height, const char* name) {
    if (!glfwInit()) {
        return NULL;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);
    if (!window) {
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return NULL;
    }
    glViewport(0, 0, width, height);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    return window;
}

void Renderer_init(Renderer *renderer) {
    glfwSetWindowUserPointer(renderer->window, renderer);

    const GLFWframebuffersizefun callbackFun = resizeCallback;
    glfwSetFramebufferSizeCallback(renderer->window, callbackFun);
    glfwSetCursorPosCallback(renderer->window, cursorPositionCallback);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    loadDefaultQuadMesh();

    ComputeShader_createUniform(&renderer->computeShader, ("dataSize"));
    ComputeShader_createUniform(&renderer->computeShader, ("thickness"));
    ComputeShader_update(&renderer->computeShader, graphingFunction);

    Shader_createUniform(&renderer->guiShader, "hasTexture");
    Shader_createUniform(&renderer->guiShader, "state");
    Shader_createUniform(&renderer->guiShader, "width");
    Shader_createUniform(&renderer->guiShader, "height");
    Shader_createUniform(&renderer->guiShader, "screenWidth");
    Shader_createUniform(&renderer->guiShader, "screenHeight");
    Shader_createUniform(&renderer->guiShader, "positionObject");
    Shader_createUniform(&renderer->guiShader, "texture_sampler");
    Shader_createUniform(&renderer->guiShader, "color");

    Shader_createUniform(&renderer->guiShader, "transformTexCoords");
    Shader_createUniform(&renderer->guiShader, "texPosStart");
    Shader_createUniform(&renderer->guiShader, "texPosEnd");
}

void Renderer_render(const Renderer *renderer) {
    ComputeShader_run(&renderer->computeShader);

    glClear(GL_COLOR_BUFFER_BIT);
    //#define DEBUG
#ifdef DEBUG
    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

    Shaders.bind(&renderer->guiShader);
    setUniform(&renderer->guiShader, ("screenWidth"), (float) renderer->screenWidth);
    setUniform(&renderer->guiShader, ("screenHeight"), (float) renderer->screenHeight);

    const Element* guiRoot = &renderer->guiRoot;

    for (int i = 0; i < guiRoot->childElements.size; i++) {
        renderElementsRecursively(guiRoot->childElements.content[i], renderer);
    }

    glfwSwapBuffers(renderer->window);
    Shaders.unbind();
}

void renderElementsRecursively(Element* element, const Renderer* renderer) {
    if (element == NULL || !element->isActive) return;

    const Shader* shader = &renderer->guiShader;

    setUniform(shader, "width", (float)element->actualWidth);
    setUniform(shader, "height", (float)element->actualHeight);

    setUniform(shader, "hasTexture", element->texture != NULL);
    setUniform(shader, "state", (int)element->state);
    element->state = 0;

    setUniform(shader, "positionObject", toVec2f(element->worldPos));
    setUniform(shader, "transformTexCoords", 0);

    setUniform(shader, "texture_sampler", 0);

    setUniform(shader, "color", element->color);

    glActiveTexture(GL_TEXTURE0);

    if (element->texture != NULL) {
        glBindTexture(GL_TEXTURE_2D, element->texture->textureId);
    } else {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Mesh_render(&element->Mesh);

    if (element->hasText) {
        renderText(renderer, element);
    }

    for (int i = 0; i < element->childElements.size; i++) {
        Element* childElement = element->childElements.content[i];
        renderElementsRecursively(childElement, renderer);
    }
}

Vec2i updateLayout(Element *element, const Vec2i parentPos, const Renderer *renderer, const int verticalOffset) {
    if (!element || !element->isActive) return (Vec2i){0,0};

    if (element->positionMode == POS_FIT) {
        element->pos.x = 0;
        element->pos.y = verticalOffset;
    }

    element->worldPos.x = parentPos.x + element->pos.x;
    element->worldPos.y = parentPos.y + element->pos.y;

    const Vec2i contentOrigin = {
        .x = element->worldPos.x + element->padding.left,
        .y = element->worldPos.y + element->padding.up
    };

    int realWidth = element->width;
    int realHeight = element->height;

    const Padding* padding = &element->padding;

    if (element->hasText) {
        const Vec2i textSize = measureText(renderer, &element->textElement);
        const int textW = padding->left + textSize.x + padding->right;
        const int textH = padding->up + textSize.y + padding->down;

        realWidth  = max(realWidth,  textW);
        realHeight = max(realHeight, textH);
    }

    int accumulatedHeight = 0;
    int maxChildWidth = 0;
    int maxChildHeight = 0;

    for (int i = 0; i < element->childElements.size; i++) {
        Element *child = element->childElements.content[i];
        const Vec2i childDimensions = updateLayout(child, contentOrigin, renderer, accumulatedHeight);

        const int childWidth = child->pos.x + childDimensions.x;
        const int childHeight = child->pos.y + childDimensions.y;

        maxChildWidth = max(maxChildWidth, childWidth);
        maxChildHeight = max(maxChildHeight, childHeight);

        accumulatedHeight += childDimensions.y + element->childGap;
    }

    realWidth  = max(realWidth,  padding->left + maxChildWidth  + padding->right);
    realHeight = max(realHeight, padding->up   + maxChildHeight + padding->down);

    element->actualWidth = realWidth;
    element->actualHeight = realHeight;

    return (Vec2i){realWidth, realHeight};
}

Vec2i measureText(const Renderer *renderer, const TextElement *textElement) {
    const Font* font = &renderer->font;
    const float scale = textElement->textScale;

    float x = 0.0f;
    float y = 0.0f;

    float maxHeight = 0.0f;

    for (int i = 0; i < textElement->text.length; i++) {
        const char c = textElement->text.content[i];
        if (c < 32 || c > 126) continue;

        stbtt_aligned_quad q;
        stbtt_GetPackedQuad(font->glyphs,
                            font->fontAtlas.width,
                            font->fontAtlas.height,
                            c - 32,
                            &x,
                            &y,
                            &q,
                            1);

        const float glyphHeight = (q.y1 - q.y0);

        maxHeight = max(maxHeight, glyphHeight);
    }

    return (Vec2i){
        (int)(x * scale),
        (int)(maxHeight * scale)
    };
}


void resizeCallback(GLFWwindow *window, const int width, const int height) {
    glViewport(0, 0, width, height);

    Renderer *renderer = (Renderer *)glfwGetWindowUserPointer(window);
    renderer->screenWidth = width;
    renderer->screenHeight = height;
}

void cursorPositionCallback(GLFWwindow* window, const double xPos, const double yPos) {
    Renderer *renderer = (Renderer *)glfwGetWindowUserPointer(window);
    renderer->mousePos.x = (int)xPos;
    renderer->mousePos.y = (int)yPos;
}

inline bool isMousePressed(GLFWwindow* window, const int mouseButton) {
    return glfwGetMouseButton(window, mouseButton) == GLFW_PRESS;
}

Renderer newRenderer(const int width, const int height, const char* name) {
    return (Renderer){
        .guiShader = newShader("GuiVertexShader.vert", "GuiFragmentShader.frag"),
        .otherShaders = (OtherShaders){0, {}},
        .computeShader = 0,
        .window = initWindow(width, height, name),
        .render = Renderer_render,
        .screenWidth = width,
        .screenHeight = height,
        .font = loadFontAtlas("ARIAL.TTF"),
        .basicQuadMesh = Mesh_loadSimpleQuad(),
        .defaultClick = NULL,
        .guiRoot = createRootElement()
    };
}

Renderer* newRenderer_h(const int width, const int height, const char* name, List_Element* elements) {
    Renderer* renderer = malloc(sizeof(Renderer));
    renderer->guiShader = newShader("GuiVertexShader.vert", "GuiFragmentShader.frag");
    renderer->window = initWindow(width, height, name);
    renderer->render = Renderer_render;
    renderer->screenWidth = width;
    renderer->screenHeight = height;
    renderer->font = loadFontAtlas("ARIAL.TTF");
    renderer->basicQuadMesh = Mesh_loadSimpleQuad();
    renderer->defaultClick = NULL;
    return renderer;
}

Element createRootElement() {
    return newElement((Mesh){}, (Vec2i){}, 0, 0, NULL);
}

void loadDefaultQuadMesh() {
    quadMesh = Mesh_loadSimpleQuad();
}


void Renderer_destroy(const Renderer *renderer) {
    glfwDestroyWindow(renderer->window);
}

