//
// Created by Stefan on 23.10.2026.
//

#pragma once
#include "GuiTypes.h"
#include "glad/gl.h"

namespace ggm {
    struct Mesh2D {
        GLuint VAO = 0;
        GLuint EBO = 0;
        GLuint vertexVBO = 0;
        GLuint textureVBO = 0;

        int indexCount = 0;

        Mesh2D() = default;

        Mesh2D(const Mesh2D& other) = delete;
        Mesh2D(Mesh2D&& other) noexcept = default;
        Mesh2D& operator=(Mesh2D&& other) noexcept = default;

        ~Mesh2D() {
            // if (VAO) glDeleteVertexArrays(1, &VAO);
            // if (EBO) glDeleteBuffers(1, &EBO);
            //
            // if (vertexVBO) glDeleteBuffers(1, &vertexVBO);
            // if (textureVBO) glDeleteBuffers(1, &textureVBO);
        }
    };
}
