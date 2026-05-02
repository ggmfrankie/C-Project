//
// Created by ertls on 18.02.2026.
//

#include "OBJLoader.hpp"
#include <cstring>
#include "../../../Utils/FileIO.hpp"
#include <filesystem>
#include <unordered_map>
#include <charconv>

#include "../Render/Mesh.hpp"
#include "Utils/TimeMeasurenments.h"
#include "Utils/DataStorage/LazyStream.hpp"
#include "Utils/DataStorage/Stream.hpp"

using std::string;
using std::string_view;
using std::vector;
using std::cout;


namespace Obj3D::OBJLoader {

    OBJObject::OBJObject(const std::string &fileName) {
        namespace fs = std::filesystem;

        fs::path filePath{};
        try {
            filePath = fs::path("..\\Resources\\Objects") / fileName;
        } catch (std::runtime_error& e) {
            std::cerr << e.what() << "inside ObjLoader\n";
        }

        mFolderPath = filePath.parent_path().string() + "\\";

        mObjFile = ggm::FileIO::readFile(filePath.string());

        mGlVertices.reserve(mObjFile.size());
        mGlUv.reserve(mObjFile.size());
        mGlNormals.reserve(mObjFile.size());

        mAllVertices.reserve(mObjFile.size());
        mAllUv.reserve(mObjFile.size());
        mAllNormals.reserve(mObjFile.size());
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
                .take(3)
                .toVector();

            if (nums.size() == 3) {
                output.emplace_back(nums[0], nums[1], nums[2]);
            }
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
                .take(2)
                .toVector();

            if (nums.size() == 2) {
                output.emplace_back(nums[0], nums[1]);
            }
        }
        return output;
    }

    void OBJObject::load() {
        mLines = ggm::LazyStream(ggm::split(mObjFile, '\n'))
                .filter([](auto &s) {return !s.empty() && !s.starts_with("#"); })
                .map([](auto line) {
                    if (!line.empty() && line.back() == '\r') line.remove_suffix(1);
                    return line;
                })
                .toVector();

        mAllVertices = convertToVec3f(getLinesWith("v "));
        mAllUv = convertToVec2f(getLinesWith("vt "));
        mAllNormals = convertToVec3f(getLinesWith("vn "));

        mMaterialLib = getMaterialLib();

        loadMeshData();
        loadMaterial();
    }

    void OBJObject::loadMeshData() {
        std::unordered_map<IdxGroup, int, IdxGroupHash> map{};
        map.reserve(mAllVertices.size()*2);
        int i = 0;
        for (auto& face: getLinesWith("f ")) {
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

                    mGlVertices.push_back(mAllVertices.at(idxGroup.v));
                    if (idxGroup.vt >= 0) mGlUv.push_back(mAllUv.at(idxGroup.vt));
                    else mGlUv.emplace_back(0.f, 0.f);

                    if (idxGroup.vn >= 0) mGlNormals.push_back(mAllNormals.at(idxGroup.vn));
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

    void OBJObject::loadMaterial() {
        if (mMaterialLib.empty()) return;
        const auto fullPath = mFolderPath + std::string(mMaterialLib);

        std::string s;
        try {
            s = ggm::FileIO::readFile(fullPath);
        } catch (std::runtime_error& e) {
            std::cerr << e.what() << " with path " << fullPath << "\n";
        }

        m_textureName = ggm::LazyStream(ggm::split(s, '\n'))
            .filter([](auto& line){return line.starts_with("map_Kd ");})
            .map([](auto line) {
                if (line.back() == '\r') line.remove_suffix(1); return line;
            })
            .map([](auto line) {
                line.remove_prefix(strlen("map_Kd ")); return std::string{line};
            })
            .getFirst();
    }

    [[nodiscard]] Mesh
    OBJObject::getMesh() {
        return {std::move(mGlVertices), std::move(mGlUv), std::move(mGlNormals), std::move(mIndices), Texture(mFolderPath + m_textureName)};
    }

    [[nodiscard]] vector<string_view>
    OBJObject::getLinesWith(const string_view &token) const {
        return ggm::LazyStream(mLines)
            .filter([token](auto& s){return s.starts_with(token);})
            .map([token](auto s) {
                s.remove_prefix(token.size()); return s;
            })
            .toVector();
    }

    [[nodiscard]] string_view
    OBJObject::getMaterialLib() const {
        return ggm::LazyStream(mLines)
            .filter([](auto& s){return s.starts_with("mtllib ");})
            .map([](auto s) {
                s.remove_prefix(strlen("mtllib ")); return s;
            })
            .getFirst();
    }
} // Loader
// Obj