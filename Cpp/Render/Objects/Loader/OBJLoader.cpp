//
// Created by ertls on 18.02.2026.
//

#include "OBJLoader.hpp"
#include <cstring>
#include "Utils/FileIO.hpp"
#include <unordered_map>
#include <charconv>

#include "../Render/Mesh.hpp"
#include "Utils/DataStorage/LazyStream.hpp"

using std::string;
using std::string_view;
using std::vector;
using std::cout;
namespace fs = std::filesystem;


namespace Obj3D::OBJLoader {
    OBJObject::OBJObject(const std::string &fileName) :
        mFolderPath(fs::current_path().parent_path() / "Resources" / "Objects" / fileName)
    {}

    /// @brief Loads the Object from the defined path and stores it internally
    void OBJObject::load() {
        const string objFile = ggm::FileIO::readFile(mFolderPath);

        const ggm::u64 size = objFile.size();
        mGlVertices.reserve(size);
        mGlUv.reserve(size);
        mGlNormals.reserve(size);

        const Lines lines = ggm::LazyStream(ggm::split(objFile, '\n'))
                .filter([](auto& s) {return !s.empty() && !s.starts_with("#"); })
                .map([](auto line) {
                    if (line.back() == '\r') line.remove_suffix(1);
                    return line;
                })
                .toVector();

        const Token mMaterialLib = getMaterialLib(lines);

        loadMeshData(lines);
        const string textureName = loadMaterial(mFolderPath.parent_path(), mMaterialLib);
        mTexture = Texture{(mFolderPath.parent_path() / textureName).string()};
    }

    void OBJObject::loadMeshData(const Lines& lines) {
        const auto vertices = convertToVec3f(getLinesWith(lines, "v "));
        const auto uvs = convertToVec2f(getLinesWith(lines, "vt "));
        const auto normals = convertToVec3f(getLinesWith(lines, "vn "));

        std::unordered_map<IdxGroup, int, IdxGroupHash> map{};
        map.reserve(vertices.size()*2);
        int i = 0;
        for (auto& face: getLinesWith(lines, "f ")) {
            auto idxGroups = loadIdxGroups(face);

            for (int numVerts = 0; auto& idxGroup: idxGroups) {
                if (numVerts > 3) {
                    mIndices.push_back(i-numVerts);
                    mIndices.push_back(i-1);
                }

                if (auto it = map.find(idxGroup); it != map.end()) {
                    mIndices.push_back(it->second);
                } else {
                    map.emplace(idxGroup, i);

                    mGlVertices.push_back(vertices.at(idxGroup.v));
                    if (idxGroup.vt >= 0) mGlUv.push_back(uvs.at(idxGroup.vt));
                    else mGlUv.emplace_back(0.f, 0.f);

                    if (idxGroup.vn >= 0) mGlNormals.push_back(normals.at(idxGroup.vn));
                    else mGlNormals.emplace_back(0.f, 1.f, 0.f);

                    mIndices.push_back(i);
                    i++;
                }
                numVerts++;
            }
        }
    }

    std::vector<OBJObject::IdxGroup>
    OBJObject::loadIdxGroups(const std::string_view &faceLine) {
        vector<IdxGroup> output;
        for (const auto& token:  ggm::split(faceLine, ' ')) {
            if (token.empty()) continue;

            auto faceIndices = ggm::split(token, '/');
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

            output.emplace_back(v, vt, vn);
        }
        return output;
    }

    std::string
    OBJObject::loadMaterial(const std::filesystem::path& path, const Token& lib) {
        if (lib.empty()) return {};
        const auto fullPath = path / lib;

        std::string s;
        try {
            s = ggm::FileIO::readFile(fullPath);
        } catch (std::runtime_error& e) {
            std::cerr << e.what() << " with path " << fullPath << "\n";
        }

        return ggm::LazyStream(ggm::split(s, '\n'))
            .filter([](auto& line){return line.starts_with("map_Kd ");})
            .map([](auto line) {
                if (line.back() == '\r') line.remove_suffix(1); return line;
            })
            .map([](auto line) {
                line.remove_prefix(strlen("map_Kd ")); return std::string{line};
            })
            .getFirst();
    }

    /// @brief Extracts the Mesh object
    [[nodiscard]] Mesh
    OBJObject::getMesh() {
        return {
          std::move(mGlVertices), 
          std::move(mGlUv), 
          std::move(mGlNormals),
          std::move(mIndices),
          std::move(mTexture)
        };
    }

    std::vector<ggm::Vector3f>
    OBJObject::convertToVec3f(const std::vector<std::string_view>& lineList) {
        vector<ggm::Vector3f> output;
        output.reserve(lineList.size());
        for (const auto& line : lineList) {
            auto nums = ggm::LazyStream(ggm::split(line, ' '))
                .map([](const auto& s) {
                    return static_cast<float>(
                        ggm::getDouble(s.data(), s.size())
                    );
                })
                .take<3>()
                .toArray();

            output.emplace_back(nums[0], nums[1], nums[2]);
        }
        return output;
    }

    std::vector<ggm::Vector2f>
    OBJObject::convertToVec2f(const vector<string_view>& lineList) {
        vector<ggm::Vector2f> output;
        output.reserve(lineList.size());
        for (const auto& line : lineList) {
            auto nums = ggm::LazyStream(ggm::split(line, ' '))
                .map([](const auto& s) {
                    return static_cast<float>(
                        ggm::getDouble(s.data(), s.size())
                    );
                })
                .take<2>()
                .toArray();

            output.emplace_back(nums[0], nums[1]);
        }
        return output;
    }

    [[nodiscard]] vector<string_view>
    OBJObject::getLinesWith(const Lines& lines, const string_view &token) {
        return ggm::LazyStream(lines)
            .filter([token](auto& s){return s.starts_with(token);})
            .map([token](auto s) {
                s.remove_prefix(token.size()); return s;
            })
            .toVector();
    }

    [[nodiscard]] string_view
    OBJObject::getMaterialLib(const Lines& lines) {
        return ggm::LazyStream(lines)
            .filter([](auto& s){return s.starts_with("mtllib ");})
            .map([](auto s) {
                s.remove_prefix(strlen("mtllib ")); return s;
            })
            .getFirst();
    }

    bool OBJObject::IdxGroup::operator==(const IdxGroup &other) const noexcept {
        return v == other.v && vt == other.vt && vn == other.vn;
    }

    std::size_t
    OBJObject::IdxGroupHash::operator()(const IdxGroup &idx) const noexcept {
        const std::size_t h1 = std::hash<int>{}(idx.v);
        const std::size_t h2 = std::hash<int>{}(idx.vt);
        const std::size_t h3 = std::hash<int>{}(idx.vn);

        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
} // Loader
// Obj
