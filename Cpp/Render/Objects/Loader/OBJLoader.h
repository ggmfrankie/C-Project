//
// Created by ertls on 18.02.2026.
//

#ifndef MIXEDPROJECT_OBJLOADER_H
#define MIXEDPROJECT_OBJLOADER_H
#include "../../../Utils/Utils.h"
#include <iostream>

#include "../../../Utils/Math/Matrix.h"
#include "../../../Utils/Math/Vector.h"

namespace Obj::Loader::OBJLoader {

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
                    auto indices = Utils::split(vertex, '/');
                    int vert = std::stoi(std::string(indices[0]));
                    int uv = indices[1].empty() ? 0 : std::stoi(std::string(indices[1]));
                    int norm = indices[2].empty() ? 0 : std::stoi(std::string(indices[2]));
                    idxGroups.emplace_back(vert, uv, norm);
                }
            }

            private:
            std::vector<IdxGroup> idxGroups;

        public:
            [[nodiscard]] const std::vector<IdxGroup>& getIdxGroups() const { return idxGroups; }
        };

        std::string folderPath;

        std::string objFile;
        std::string materialFile;
        std::string textureName;
        std::string_view materialLib;

        std::vector<std::string_view> lines;
        std::vector<std::string_view> vertexLines;
        std::vector<std::string_view> textureLines;
        std::vector<std::string_view> normalsLines;
        std::vector<std::string_view> facesLines;

        std::vector<Face> faces;
        std::vector<Math::Vector3f> vertices;
        std::vector<Math::Vector3f> normals;
        std::vector<Math::Vector2f> uv;
        std::vector<int> indices;

        [[nodiscard]] std::vector<std::string_view> getLinesWith(const std::string_view &token) const;

        [[nodiscard]] std::string_view getMaterialLib() const;

        static std::vector<Math::Vector3f> convertToVec3f(const std::vector<std::string_view> &lineList);

    public:
        friend std::ostream& operator<<(std::ostream& os, const OBJObject& o);
    };

}

#endif //MIXEDPROJECT_OBJLOADER_H
