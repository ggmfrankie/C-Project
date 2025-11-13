//
// Created by Stefan on 10.10.2025.
//
#include "glad/gl.h"
#include "Render.h"

#include <math.h>

#include "Engine.h"
#include "../Utils/Vector.h"


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

    ComputeShader_createUniform(&renderer->computeShader, stringOf("dataSize"));
    ComputeShader_createUniform(&renderer->computeShader, stringOf("thickness"));
    ComputeShader_update(&renderer->computeShader, graphingFunction);

    Shader_createUniform(&renderer->shader ,stringOf("hasTexture"));
    Shader_createUniform(&renderer->shader, stringOf("state"));
    Shader_createUniform(&renderer->shader, stringOf("width"));
    Shader_createUniform(&renderer->shader, stringOf("height"));
    Shader_createUniform(&renderer->shader, stringOf("screenWidth"));
    Shader_createUniform(&renderer->shader, stringOf("screenHeight"));
    Shader_createUniform(&renderer->shader, stringOf("positionObject"));
    Shader_createUniform(&renderer->shader, stringOf("texture_sampler"));
}

void setUniform_f(const Shader *shader, const String name, const float value) {
    glUniform1f(Uniforms_Map_get(&shader->uniforms, name), value);
}

void setUniform_i(const Shader *shader, const String name, const int value) {
    glUniform1i(Uniforms_Map_get(&shader->uniforms, name), value);
}

void setUniform_Vec2(const Shader *shader, const String name, const Vec2f value) {
    glUniform2f(Uniforms_Map_get(&shader->uniforms, name), value.x, value.y);
}

Renderer newRenderer(const int width, const int height, const char* name, const List_Element elements) {
    return (Renderer){
        .elements = elements,
        .shader = newShader(),
        .computeShader = 0,
        .window = initWindow(width, height, name),
        .render = Renderer_render,
        .screenWidth = width,
        .screenHeight = height
    };
}

void Renderer_destroy(const Renderer *renderer) {
    glfwDestroyWindow(renderer->window);
}

void Renderer_render(Renderer *renderer) {
    ComputeShader_run(&renderer->computeShader);

    glClear(GL_COLOR_BUFFER_BIT);

    #ifdef DEBUG
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    #endif

    renderer->shader.bind(&renderer->shader);
    setUniform_f(&renderer->shader, stringOf("screenWidth"), (float) renderer->screenWidth);
    setUniform_f(&renderer->shader, stringOf("screenHeight"), (float) renderer->screenHeight);

    for (int i = 0; i < renderer->elements.size; i++) {
        const Element *element = Element_ListGet_ptr(&renderer->elements, i);
        if (element == NULL) break;
        const Shader *shader = &renderer->shader;

        setUniform_f(shader, stringOf("width"), element->width);
        setUniform_f(shader, stringOf("height"), element->height);

        setUniform_i(shader, stringOf("hasTexture"), 1);
        setUniform_i(shader, stringOf("state"), element->state);

        setUniform_Vec2(shader, stringOf("positionObject"), element->pos);

        glActiveTexture(GL_TEXTURE0);
        if (element->texture != NULL) {
            glBindTexture(GL_TEXTURE_2D, element->texture->textureId);
        }

        setUniform_i(shader, stringOf("texture_sampler"), 0);
        Mesh_render(element->Mesh);
    }

    glfwSwapBuffers(renderer->window);
    renderer->shader.unbind();
}
