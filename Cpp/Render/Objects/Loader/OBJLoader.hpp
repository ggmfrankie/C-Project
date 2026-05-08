//
// Created by ertls on 18.02.2026.
//

#pragma once
#include "../EngineDefines.hpp"
#include "../../../Utils/Utils.hpp"
#include <iostream>
#include <filesystem>

#include "../../../Utils/Math/Vector.hpp"
#include "../Render/Mesh.hpp"

namespace Obj3D::OBJLoader {

    class OBJObject {
    public:
        explicit OBJObject(const std::string& fileName);

        void load();

        Mesh getMesh();

    private:
        struct IdxGroup {
            int v = 0;
            int vt = 0;
            int vn = 0;

            bool operator==(const IdxGroup& other) const noexcept;
        };

        struct IdxGroupHash {
            std::size_t operator()(const IdxGroup& idx) const noexcept;
        };

        std::filesystem::path mFolderPath{};

        std::vector<ggm::Vector3f> mGlVertices{};
        std::vector<ggm::Vector3f> mGlNormals{};
        std::vector<ggm::Vector2f> mGlUv{};

        std::vector<GLuint> mIndices{};

        Texture mTexture{};

        using Token = std::string_view;
        using Lines = std::vector<Token>;

        [[nodiscard]] static std::vector<std::string_view>
            getLinesWith(const Lines& lines, const Token &token);

        [[nodiscard]] static std::string_view
            getMaterialLib(const Lines& lines);

        [[nodiscard]] static std::vector<ggm::Vector3f>
            convertToVec3f(const Lines &lineList);

        [[nodiscard]] static std::vector<ggm::Vector2f>
            convertToVec2f(const Lines &lineList);

        [[nodiscard]] static std::vector<IdxGroup>
            loadIdxGroups(const Token &faceLine);

        [[nodiscard]] static std::string
            loadMaterial(const std::filesystem::path& path, const Token &lib);

        void loadMeshData(const Lines &lines);
    };

}

