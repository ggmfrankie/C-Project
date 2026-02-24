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

        friend std::ostream& operator<<(std::ostream& os, const OBJObject& o);
        Mesh getMesh();

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
                for (const auto& token:  Utils::split(faceLine, ' ')) {
                    if (token.empty()) continue;

                    auto faceIndices = Utils::split(token, '/');
                    int v = -1;
                    int vt = -1;
                    int vn = -1;

                    if (!faceIndices[0].empty()) {
                        auto& s = faceIndices[0];
                        std::from_chars(s.data(), s.data() + s.size(), v);
                        v -= 1;
                    }

                    if (faceIndices.size() > 1 && !faceIndices[1].empty()) {
                        auto& s = faceIndices[1];
                        std::from_chars(s.data(), s.data() + s.size(), vt);
                        vt -= 1;
                    }

                    if (faceIndices.size() > 2 && !faceIndices[2].empty()) {
                        auto& s = faceIndices[2];
                        std::from_chars(s.data(), s.data() + s.size(), vn);
                        vn -= 1;
                    }

                    idxGroups.emplace_back(v, vt, vn);
                }
            }

            private:
            std::vector<IdxGroup> idxGroups{};

        public:
            [[nodiscard]] const std::vector<IdxGroup>& getIdxGroups() const { return idxGroups; }
        };

        std::string m_folderPath{};

        std::string m_objFile{};
        std::string m_materialFile{};
        std::string m_textureName{};
        std::string_view m_materialLib{};

        std::vector<std::string_view> m_lines{};

        std::vector<Face> m_faces{};
        std::vector<Math::Vector3f> m_allVertices{};
        std::vector<Math::Vector3f> m_allNormals{};
        std::vector<Math::Vector2f> m_allUv{};

        std::vector<Math::Vector3f> m_glVertices{};
        std::vector<Math::Vector3f> m_glNormals{};
        std::vector<Math::Vector2f> m_glUv{};

        std::vector<GLuint> m_indices{};

        [[nodiscard]] std::vector<std::string_view> getLinesWith(const std::string_view &token) const;

        [[nodiscard]] std::string_view getMaterialLib() const;

        static auto convertToVec3f(const std::vector<std::string_view> &lineList);

        static auto convertToVec2f(const std::vector<std::string_view> &lineList);

        void loadMeshData();

        std::string_view loadMaterial();

    };

}

#endif //MIXEDPROJECT_OBJLOADER_H
