//
// Created by ertls on 16.02.2026.
//

#include "../Geometry/Mesh.h"
namespace Obj {
    using std::string, std::vector;

    Mesh::Mesh(const string &texture) : texture(texture), material(){
        vertices.reserve(64);
        normals.reserve(64);
        uvs.reserve(64);
        indices.reserve(64);
        Math::Vector3f verts[] = {
            {0.5f, 0.5f, 0.0f},
            {0.5f, -0.5f, 0.0f},
            {-0.5f, -0.5f, 0.0f},
            {-0.5f, 0.5f, 0.0f}
        };

        Math::Vector2f tex[] = {
            {0.0f, 0.0f},
            {0.5f, 0.5f},
            {0.5f, 0.5f},
            {0.5f, 1.0f}
        };


        unsigned int indis[] = {
            0, 3, 1,
            1, 3, 2
        };

        vertices.insert(vertices.end(), std::begin(verts), std::end(verts));
        uvs.insert(uvs.end(), std::begin(tex), std::end(tex));
        indices.insert(indices.end(), std::begin(indis), std::end(indis));
    }

    Mesh::Mesh(const std::vector<Math::Vector3f> &v, const std::vector<GLuint> &i) : texture(), material(), VBOs(){
        vertices = v;
        indices = i;
    }

    Mesh::Mesh(): VBOs() {
        vertices.reserve(64);
        normals.reserve(64);
        uvs.reserve(64);
        indices.reserve(64);

        puts("creating default mesh");

        Math::Vector3f verts[] = {
            {0.5f, 0.5f, 0.0f},
            {0.5f, -0.5f, 0.0f},
            {-0.5f, -0.5f, 0.0f},
            {-0.5f, 0.5f, 0.0f}
        };


        unsigned int indis[] = {
            0, 3, 1,
            1, 3, 2
        };


        Math::Vector2f tex[] = {
            {0.0f, 0.0f},
            {0.5f, 0.5f},
            {0.5f, 0.5f},
            {0.5f, 1.0f}
        };

        vertices.insert(vertices.end(), std::begin(verts), std::end(verts));
        uvs.insert(uvs.end(), std::begin(tex), std::end(tex));
        indices.insert(indices.end(), std::begin(indis), std::end(indis));
    }

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
          indices(std::move(other.indices))
    {
        other.initialized = false;
        other.VAO = 0;
        other.EBO = 0;
        other.VBOs = {0,0,0};
    }

    Mesh::~Mesh() {
        if (initialized) glDeleteVertexArrays(1, &VAO);
    }

    void Mesh::init() {
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
        indices = {};

        initialized = true;
        texture.init();
        material.init();
    }

    void Mesh::render() const {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }
}

