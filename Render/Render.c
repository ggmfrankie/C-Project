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

void initRenderer(Renderer *renderer) {

}

void destroyGL(const Renderer *renderer) {
    glfwDestroyWindow(renderer->window);
}

void render(Renderer *renderer) {
    glClear(GL_COLOR_BUFFER_BIT);
    bindShaderProgram(renderer->shader.programId);

    for (int i = 0; i < renderer->meshes.size; i++) {
        const Mesh *mesh = Mesh_ListGet_ptr(&renderer->meshes, i);
        renderMesh(mesh);
    }

    glfwSwapBuffers(renderer->window);
    bindShaderProgram(0);
}