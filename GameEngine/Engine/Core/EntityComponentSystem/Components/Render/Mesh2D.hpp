//
// Created by Stefan on 23.10.2026.
//

#pragma once
#include <atomic>
#include "glad/gl.h"

namespace ggm {
    struct Mesh2D {
        GLuint VAO = 0;
        GLuint EBO = 0;
        GLuint vertexVBO = 0;
        GLuint textureVBO = 0;

        int indexCount = 0;

        std::atomic_size_t* refCount = new std::atomic_size_t{1};

        Mesh2D() = default;

        Mesh2D(const Mesh2D& other) noexcept :
            VAO(other.VAO),
            EBO(other.EBO),
            vertexVBO(other.vertexVBO),
            textureVBO(other.textureVBO),
            indexCount(other.indexCount),
            refCount(other.refCount)
        {
            if (refCount) refCount->fetch_add(1, std::memory_order_relaxed);
        };

        Mesh2D& operator=(const Mesh2D& other) noexcept {
            if (&other == this) return *this;

            VAO = other.VAO;
            EBO = other.EBO;
            vertexVBO = other.vertexVBO;
            textureVBO = other.textureVBO;
            indexCount = other.indexCount;
            refCount = other.refCount;

            if (refCount) refCount->fetch_add(1, std::memory_order_relaxed);

            return *this;
        }

        Mesh2D(Mesh2D&& other) noexcept :
            VAO(other.VAO),
            EBO(other.EBO),
            vertexVBO(other.vertexVBO),
            textureVBO(other.textureVBO),
            indexCount(other.indexCount),
            refCount(other.refCount)
        {
            other.VAO = 0;
            other.EBO = 0;
            other.vertexVBO = 0;
            other.textureVBO = 0;
            other.indexCount = 0;
            other.refCount = nullptr;
        }

        Mesh2D& operator=(Mesh2D&& other) noexcept {
            if (&other == this) return *this;

            VAO = other.VAO;
            EBO = other.EBO;
            vertexVBO = other.vertexVBO;
            textureVBO = other.textureVBO;
            indexCount = other.indexCount;
            refCount = other.refCount;

            other.VAO = 0;
            other.EBO = 0;
            other.vertexVBO = 0;
            other.textureVBO = 0;
            other.indexCount = 0;
            other.refCount = nullptr;

            return *this;
        }

        ~Mesh2D() {
            if (refCount && --*refCount == 0) {
                delete refCount;

                if (VAO) glDeleteVertexArrays(1, &VAO);
                if (EBO) glDeleteBuffers(1, &EBO);

                if (vertexVBO) glDeleteBuffers(1, &vertexVBO);
                if (textureVBO) glDeleteBuffers(1, &textureVBO);
            }
        }
    };
}
