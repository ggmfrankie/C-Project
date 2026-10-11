//
// Created by ertls on 18.02.2026.
//

#pragma once
#include "EngineDefines.hpp"
#include "Utils/StringUtils.hpp"
#include <iostream>
#include <filesystem>

#include "Utils/Math/Vector.hpp"
#include "../Render/Mesh3D.hpp"

namespace ggm::OBJLoader {

    class OBJObject {
    public:
        explicit OBJObject(const std::filesystem::path& fileName);

        void load();

        Mesh3D getMesh();

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

        std::filesystem::path mFolderPath;

        std::vector<Vector3f> mGlVertices;
        std::vector<Vector3f> mGlNormals;
        std::vector<Vector2f> mGlUv;

        std::vector<GLuint> mIndices;

        Texture3D mTexture;

        using Token = std::string_view;
        using Lines = std::vector<Token>;

        [[nodiscard]] static
        auto getLinesWith(const Lines &lines, const Token &token) -> std::vector<std::string_view>;

        [[nodiscard]] static
        auto getMaterialLib(const Lines &lines) -> std::string_view;

        [[nodiscard]] static
        auto convertToVec3f(const Lines &lineList) -> std::vector<Vector3f>;

        [[nodiscard]] static
        auto convertToVec2f(const Lines &lineList) -> std::vector<Vector2f>;

        [[nodiscard]] static
        auto loadIdxGroups(const Token &faceLine) -> std::vector<IdxGroup>;

        [[nodiscard]] static
        auto loadMaterial(const std::filesystem::path &path, const Token &lib) -> std::string;

        void loadMeshData(const Lines &lines);
    };

}

