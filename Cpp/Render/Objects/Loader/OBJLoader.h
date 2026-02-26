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

        std::string m_folderPath{};

        std::string m_objFile{};
        std::string m_materialFile{};
        std::string m_textureName{};
        std::string_view m_materialLib{};

        std::vector<std::string_view> m_lines{};

        std::vector<Math::Vector3f> m_allVertices{};
        std::vector<Math::Vector3f> m_allNormals{};
        std::vector<Math::Vector2f> m_allUv{};

        std::vector<Math::Vector3f> m_glVertices{};
        std::vector<Math::Vector3f> m_glNormals{};
        std::vector<Math::Vector2f> m_glUv{};

        std::vector<GLuint> m_indices{};

        [[nodiscard]] std::vector<std::string_view> getLinesWith(const std::string_view &token) const;

        [[nodiscard]] std::string_view getMaterialLib() const;

        static std::vector<Math::Vector3f> convertToVec3f(const std::vector<std::string_view> &lineList);

        static std::vector<Math::Vector2f> convertToVec2f(const std::vector<std::string_view> &lineList);

        void loadMeshData();

        static std::vector<IdxGroup> loadIdxGroups(const std::string_view &faceLine);

        void loadMaterial();

    };

}

#endif //MIXEDPROJECT_OBJLOADER_H
