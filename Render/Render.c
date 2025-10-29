//
// Created by Stefan on 10.10.2025.
//
#include "glad/gl.h"
#include "Render.h"
#include "../Utils/Vector.h"


GLFWwindow* initWindow(const int width, const int height, const char* name) {

    if (!glfwInit()) {
        return NULL;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
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
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    return window;
}

void resizeCallback(GLFWwindow *window, const int width, const int height) {
    glViewport(0, 0, width, height);
    Renderer *renderer = (Renderer *)glfwGetWindowUserPointer(window);
    renderer->screenWidth = width;
    renderer->screenHeight = height;
}

void Renderer_init(Renderer *renderer) {
    glfwSetWindowUserPointer(renderer->window, renderer);

    const GLFWframebuffersizefun callbackFun = resizeCallback;
    glfwSetFramebufferSizeCallback(renderer->window, callbackFun);
    Shader_createUniform(&renderer->shader ,newString("hasTexture"));
    Shader_createUniform(&renderer->shader, newString("isActive"));
    Shader_createUniform(&renderer->shader, newString("width"));
    Shader_createUniform(&renderer->shader, newString("height"));
    Shader_createUniform(&renderer->shader, newString("screenWidth"));
    Shader_createUniform(&renderer->shader, newString("screenHeight"));
    Shader_createUniform(&renderer->shader, newString("positionObject"));
}

void setUniform_f(const Shader *shader, const String name, const float value) {
    //name.println(&name);
    //printf("Int value inside map is: %i\n", Uniforms_Map_get(&shader->uniforms, name));
    glUniform1f(Uniforms_Map_get(&shader->uniforms, name), value);
}

void setUniform_i(const Shader *shader, const String name, const int value) {
    glUniform1i(Uniforms_Map_get(&shader->uniforms, name), value);
}

void setUniform_Vec2(const Shader *shader, const String name, const Vec2f value) {
    glUniform2f(Uniforms_Map_get(&shader->uniforms, name), value.x, value.y);
}

Renderer newRenderer(const int width, const int height, const char* name, const Element_Array elements) {
    return (Renderer){
        .elements = elements,
        .shader = newShader(),
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
    glClear(GL_COLOR_BUFFER_BIT);
    renderer->shader.bind(&renderer->shader);
    setUniform_f(&renderer->shader, newString("screenWidth"), (float) renderer->screenWidth);
    setUniform_f(&renderer->shader, newString("screenHeight"), (float) renderer->screenHeight);

    for (int i = 0; i < renderer->elements.size; i++) {
        const Element *element = Element_Array_get(&renderer->elements, i);
        const Shader *shader = &renderer->shader;

        setUniform_f(shader, newString("width"), element->width);
        setUniform_f(shader, newString("height"), element->height);

        setUniform_i(shader, newString("hasTexture"), 0);
        setUniform_i(shader, newString("isActive"), 1);

        setUniform_Vec2(shader, newString("positionObject"), element->pos);
        Mesh_render(element->Mesh);
    }
    glfwSwapBuffers(renderer->window);
    renderer->shader.unbind();
}
