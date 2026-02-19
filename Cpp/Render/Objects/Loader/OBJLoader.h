//
// Created by ertls on 18.02.2026.
//

#ifndef MIXEDPROJECT_OBJLOADER_H
#define MIXEDPROJECT_OBJLOADER_H
#include "../../../Utils/Utils.h"
#include <iostream>

#include "../../../Utils/Math/Matrix.h"
#include "../../../Utils/Math/Vector.h"
#include "../Geometry/Mesh.h"

namespace Obj::OBJLoader {

    class OBJObject {
    public:
        explicit OBJObject(const std::string& fileName);

        void load();

    private:
        class Face {
        public:
            struct IdxGroup {
                int v = 0;
                int vt = 0;
                int vn = 0;

                bool operator==(const IdxGroup& other) const noexcept {
                    return v == other.v && vt == other.vt && vn == other.vn;
                }
            };

            struct IdxGroupHash {
                std::size_t operator()(const IdxGroup& idx) const noexcept {

                    const std::size_t h1 = std::hash<int>{}(idx.v);
                    const std::size_t h2 = std::hash<int>{}(idx.vt);
                    const std::size_t h3 = std::hash<int>{}(idx.vn);

                    return h1 ^ (h2 << 1) ^ (h3 << 2);
                }
            };

            explicit Face(const std::string_view &faceLine) {
                for (const auto& vertexLine = Utils::split(faceLine, ' '); const auto& vertex: vertexLine) {
                    auto faceIndices = Utils::split(vertex, '/');
                    int faceVert = 0;
                    try {
                         faceVert = std::stoi(std::string(faceIndices[0]));
                    } catch (const std::exception&) {
                        std::cout << "invalid string caught: " <<faceIndices[0] << "\n";
                        std::cout << faceLine << "\n";
                        std::cout << vertex << "\n";
                    }

                    int faceUv = faceIndices[1].empty() ? 0 : std::stoi(std::string(faceIndices[1]));
                    int faceNorm = faceIndices[2].empty() ? 0 : std::stoi(std::string(faceIndices[2]));
                    idxGroups.emplace_back(faceVert, faceUv, faceNorm);
                }
            }

            private:
            std::vector<IdxGroup> idxGroups{};

        public:
            [[nodiscard]] const std::vector<IdxGroup>& getIdxGroups() const { return idxGroups; }
        };

        std::string folderPath{};

        std::string objFile{};
        std::string materialFile{};
        std::string textureName{};
        std::string_view materialLib{};

        std::vector<std::string_view> lines{};

        std::vector<Face> faces{};
        std::vector<Math::Vector3f> allVertices{};
        std::vector<Math::Vector3f> allNormals{};
        std::vector<Math::Vector2f> allUv{};

        std::vector<Math::Vector3f> glVertices{};
        std::vector<Math::Vector3f> glNormals{};
        std::vector<Math::Vector2f> glUv{};

        std::vector<GLuint> indices{};

        [[nodiscard]] std::vector<std::string_view> getLinesWith(const std::string_view &token) const;

        [[nodiscard]] std::string_view getMaterialLib() const;

        static auto convertToVec3f(const std::vector<std::string_view> &lineList);

        static auto convertToVec2f(const std::vector<std::string_view> &lineList);

    public:
        friend std::ostream& operator<<(std::ostream& os, const OBJObject& o);
        Mesh getMesh();
    };

}

#endif //MIXEDPROJECT_OBJLOADER_H
