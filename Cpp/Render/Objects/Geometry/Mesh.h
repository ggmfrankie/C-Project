//
// Created by ertls on 16.02.2026.
//

#ifndef MIXEDPROJECT_MESH_H
#define MIXEDPROJECT_MESH_H
#include <vector>
#include <array>

#include "Material.h"
#include "Texture.h"
#include "../../../Utils/Math/Vector.h"
#include "glad/gl.h"

namespace Obj::Geometry {
    class Mesh {
    public:
        explicit Mesh(const std::string &texture);

        Mesh(const std::vector<Math::Vector3f> &v, const std::vector<GLuint> &i);

        Mesh();

        ~Mesh();

        void init();

        void render() const;
    private:
        bool initialized = false;

        GLuint VAO;
        GLuint EBO;

        Texture texture;
        Material material;

        std::array<GLuint, 3> VBOs;
        std::vector<Math::Vector3f> vertices;
        std::vector<Math::Vector3f> normals;
        std::vector<Math::Vector2f> texCoords;
        std::vector<GLuint> indices;
    };
}

#endif //MIXEDPROJECT_MESH_H