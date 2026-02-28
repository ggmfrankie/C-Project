//
// Created by ertls on 16.02.2026.
//

#include "../Geometry/Mesh.hpp"
#include "../../Cpp/Utils/Math/Vector.hpp"

namespace Obj {
    using std::string, std::vector;
    Mesh::Mesh(std::vector<Math::Vector3f>&& vertices,
                   std::vector<Math::Vector2f>&& uvs,
                   std::vector<Math::Vector3f>&& normals,
                   std::vector<GLuint>&& indices,
                   Texture&& texture)
            : texture(std::move(texture)),
              vertices(std::move(vertices)),
              normals(std::move(normals)),
              uvs(std::move(uvs)),
              indices(std::move(indices))
    {}

    Mesh::Mesh(std::vector<Math::Vector3f>&& vertices,
                   std::vector<Math::Vector2f>&& uvs,
                   std::vector<Math::Vector3f>&& normals,
                   std::vector<GLuint>&& indices)
            : texture(),
              vertices(std::move(vertices)),
              normals(std::move(normals)),
              uvs(std::move(uvs)),
              indices(std::move(indices))
    {}


    Mesh::Mesh(Mesh&& other) noexcept
        : initialized(other.initialized),
          VAO(other.VAO),
          EBO(other.EBO),
          texture(std::move(other.texture)),
          material(other.material),
          VBOs(other.VBOs),
          vertices(std::move(other.vertices)),
          normals(std::move(other.normals)),
          uvs(std::move(other.uvs)),
          indices(std::move(other.indices)) {

        other.initialized = false;
        other.VAO = 0;
        other.EBO = 0;
        other.VBOs = {0, 0, 0};
    }

    Mesh::~Mesh() {
        if (initialized) glDeleteVertexArrays(1, &VAO);
    }

    void Mesh::init(Render::Shader* s) {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glGenBuffers(3, VBOs.data());

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size() * 3, vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normals.size() * 3, normals.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uvs.size() * 2, uvs.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);


        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        vertices = {};
        uvs = {};
        normals = {};

        shader = s;

        puts("init mesh");

        if (texture.hasData()) texture.init();
        if (material.hasData()) material.init();

        initialized = true;
    }

    void Mesh::render() const {
        shader->setUniform("textureSampler", 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.id());

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    bool Mesh::hasTexture() const {
        return texture.hasData();
    }

    bool Mesh::hasMaterial() const {
        return material.hasData();
    }

    Mesh Mesh::getDummyMesh() {
        vector<Math::Vector3f> vertices = {
            {0.5f, 0.5f, 0.0f},
            {0.5f, -0.5f, 0.0f},
            {-0.5f, -0.5f, 0.0f},
            {-0.5f, 0.5f, 0.0f}
        };

        vector<Math::Vector3f> normals = {
            {0.5f, 0.5f, 0.0f},
            {0.5f, -0.5f, 0.0f},
            {-0.5f, -0.5f, 0.0f},
            {-0.5f, 0.5f, 0.0f}
        };

        vector<GLuint> indis = {
            0, 3, 1,
            1, 3, 2
        };

        vector<Math::Vector2f> tex = {
            {1.0f, 1.0f},
            {1.0f, 0.0f},
            {0.0f, 0.0f},
            {0.0f, 1.0f}
        };

        Texture texture("../Resources/Objects/grass_block/grass_block.png");
        puts("load dummy Mesh");

        return {std::move(vertices), std::move(tex), std::move(normals), std::move(indis), std::move(texture)};
    }
}

