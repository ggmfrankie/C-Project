//
// Created by ertls on 16.02.2026.
//

#ifndef MIXEDPROJECT_MESH_H
#define MIXEDPROJECT_MESH_H
#include <vector>
#include <array>
#include <memory>

#include "Material.hpp"
#include "Texture.hpp"
#include "../../../Utils/Math/Vector.hpp"
#include "glad/gl.h"
#include "../../Shader/Shader.hpp"

namespace Obj {
    class Mesh {
    public:
        Mesh(std::vector<Math::Vector3f> &&vertices, std::vector<Math::Vector2f> &&uvs,
             std::vector<Math::Vector3f> &&normals,
             std::vector<GLuint> &&indices, Texture &&texture);

        Mesh(std::vector<Math::Vector3f> &&vertices, std::vector<Math::Vector2f> &&uvs,
             std::vector<Math::Vector3f> &&normals,
             std::vector<GLuint> &&indices);

        Mesh(Mesh &&other) noexcept;

        ~Mesh();

        void init(Render::Shader *);

        void render() const;

        [[nodiscard]] bool hasTexture() const;

        [[nodiscard]] bool hasMaterial() const;

        static Mesh getDummyMesh();

    private:
        bool initialized = false;

        GLuint VAO = 0;
        GLuint EBO = 0;

        Texture texture{};
        Material material{};

        std::array<GLuint, 3> VBOs{};
        std::vector<Math::Vector3f> vertices{};
        std::vector<Math::Vector3f> normals{};
        std::vector<Math::Vector2f> uvs{};
        std::vector<GLuint> indices{};

        Render::Shader* shader = nullptr;
    };
}

#endif //MIXEDPROJECT_MESH_H