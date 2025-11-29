//
// Created by Stefan on 10.10.2025.
//
#include "glad/gl.h"
#include "Render.h"

#include <math.h>

#include "Engine.h"
#include "../Utils/Vector.h"

void loadDefaultQuadMesh();

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

void resizeCallback(GLFWwindow *window, const int width, const int height) {

    glViewport(0, 0, width, height);

    Renderer *renderer = (Renderer *)glfwGetWindowUserPointer(window);
    renderer->screenWidth = width;
    renderer->screenHeight = height;
}

void cursorPositionCallback(GLFWwindow* window, const double xPos, const double yPos) {
    Renderer *renderer = (Renderer *)glfwGetWindowUserPointer(window);
    renderer->mousePos.x = (float)xPos;
    renderer->mousePos.y = (float)yPos;
}

inline bool isMousePressed(GLFWwindow* window, const int mouseButton) {
    return glfwGetMouseButton(window, mouseButton) == GLFW_PRESS;
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

    Shader_createUniform(&renderer->shader, "hasTexture");
    Shader_createUniform(&renderer->shader, "state");
    Shader_createUniform(&renderer->shader, "width");
    Shader_createUniform(&renderer->shader, "height");
    Shader_createUniform(&renderer->shader, "screenWidth");
    Shader_createUniform(&renderer->shader, "screenHeight");
    Shader_createUniform(&renderer->shader, "positionObject");
    Shader_createUniform(&renderer->shader, "texture_sampler");

    Shader_createUniform(&renderer->shader, "transformTexCoords");
    Shader_createUniform(&renderer->shader, "texPosStart");
    Shader_createUniform(&renderer->shader, "texPosEnd");
}



Renderer newRenderer(const int width, const int height, const char* name, List_Element* elements) {
    return (Renderer){
        .elements = elements,
        .shader = newShader(),
        .computeShader = 0,
        .window = initWindow(width, height, name),
        .render = Renderer_render,
        .screenWidth = width,
        .screenHeight = height,
        .font = loadFontAtlas("From Cartoon Blocks.ttf"),
        .basicQuadMesh = Mesh_loadSimpleQuad(),
        .defaultClick = NULL
    };
}

Renderer* newRenderer_h(const int width, const int height, const char* name, List_Element* elements) {
    Renderer* renderer = malloc(sizeof(Renderer));
    renderer->elements = elements;
    renderer->shader = newShader();
    renderer->window = initWindow(width, height, name);
    renderer->render = Renderer_render;
    renderer->screenWidth = width;
    renderer->screenHeight = height;
    renderer->font = loadFontAtlas("ARIAL.TTF");
    renderer->basicQuadMesh = Mesh_loadSimpleQuad();
    renderer->defaultClick = NULL;
    return renderer;
}

void guiAddElement(
    List_Element* list,
    const Mesh mesh,
    const short meshCount,
    const Vec2f pos,
    const int width,
    const int height,
    Texture* tex,
    bool (*mouseOver)(const Element*, Vec2f),
    bool (*hover)(Element*, Renderer*),
    bool (*click)(Element*, Renderer*),
    const char* text,
    const bool forceResize
    )
{
    Element_ListAdd(list, newElement(mesh, meshCount, pos, width, height, tex));
    Element* lastElement = Element_ListGetLast(list);
    if (mouseOver) {
        lastElement->isMouseOver = mouseOver;
        if (hover) lastElement->onHover = hover;
        if (click) lastElement->onClick = click;
    }
    if (text) {

        lastElement->textElement = (TextElement) {
            .offset = (Vec2f){0.2f, 0.2f},
            .text = newReservedString(512),
            .textScale = 1.0f,
            .textColor = (Vec3f){1.0f, 1.0f, 1.0f},
            .forceResize = forceResize
        };
        setText(lastElement, text);
        lastElement->hasText = true;
    }
}

void guiAddSimpleRectangle(
    List_Element* list,
    const Vec2f pos,
    const int width,
    const int height,
    Texture* tex
    )
{
    guiAddElement(list, quadMesh, 1, pos, width, height, tex, NULL, NULL, NULL, NULL, false);
}

void guiAddSimpleButton(
    List_Element* list,
    const Vec2f pos,
    const int width,
    const int height,
    Texture* tex,
    bool (*hover)(Element*, Renderer*),
    bool (*click)(Element*, Renderer*),
    const char* text
    )
{
    guiAddElement(list, quadMesh, 1, pos, width, height, tex, isSelected_Quad, hover, click, text, true);
}

void loadDefaultQuadMesh() {
    quadMesh = Mesh_loadSimpleQuad();
}


void Renderer_destroy(const Renderer *renderer) {
    glfwDestroyWindow(renderer->window);
}

void Renderer_render(const Renderer *renderer) {
    ComputeShader_run(&renderer->computeShader);

    glClear(GL_COLOR_BUFFER_BIT);
//#define DEBUG
    #ifdef DEBUG
    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    #endif

    Shaders.bind(&renderer->shader);
    setUniform_f(&renderer->shader, ("screenWidth"), (float) renderer->screenWidth);
    setUniform_f(&renderer->shader, ("screenHeight"), (float) renderer->screenHeight);

    for (int i = 0; i < renderer->elements->size; i++) {
        const Element *element = Element_ListGet_ptr(renderer->elements, i);
        if (element == NULL) break;
        const Shader *shader = &renderer->shader;

        setUniform_f(shader, ("width"), element->width);
        setUniform_f(shader, ("height"), element->height);

        setUniform_i(shader, ("hasTexture"), 1);
        setUniform_i(shader, ("state"), element->state);

        setUniform_Vec2(shader, ("positionObject"), element->pos);
        setUniform_i(shader, ("transformTexCoords"), 0);

        glActiveTexture(GL_TEXTURE0);
        if (element->texture != NULL) {
            glBindTexture(GL_TEXTURE_2D, element->texture->textureId);
        }

        setUniform_i(shader, ("texture_sampler"), 0);
        Mesh_render(&element->Mesh);

        if (element->hasText) {
            renderText(renderer, element);
        }
    }

    glfwSwapBuffers(renderer->window);
    Shaders.unbind();
}
