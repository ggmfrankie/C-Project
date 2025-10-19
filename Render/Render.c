//
// Created by Stefan on 10.10.2025.
//
#include "glad/gl.h"
#include "Render.h"


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

void Renderer_init(const Renderer *renderer) {
    renderer->shader.createUniform(&renderer->shader ,newString("hasTexture"));
    renderer->shader.createUniform(&renderer->shader, newString("isActive"));
    renderer->shader.createUniform(&renderer->shader, newString("width"));
    renderer->shader.createUniform(&renderer->shader, newString("height"));
    renderer->shader.createUniform(&renderer->shader, newString("screenWidth"));
    renderer->shader.createUniform(&renderer->shader, newString("screenHeight"));
}

Renderer newRenderer(const int width, const int height, const char* name) {
    return (Renderer){
        .meshes = Mesh_newList(1),
        .shader = newShader(),
        .window = initWindow(width, height, name),
        .render = Renderer_render
    };
}

void Renderer_destroy(const Renderer *renderer) {
    glfwDestroyWindow(renderer->window);
}

void Renderer_render(Renderer *renderer) {
    glClear(GL_COLOR_BUFFER_BIT);
    renderer->shader.bind(&renderer->shader);

    for (int i = 0; i < renderer->meshes.size; i++) {
        const Mesh *mesh = renderer->meshes.get(&renderer->meshes, i);
        mesh->render(mesh);
    }
    glfwSwapBuffers(renderer->window);
    renderer->shader.unbind();
}