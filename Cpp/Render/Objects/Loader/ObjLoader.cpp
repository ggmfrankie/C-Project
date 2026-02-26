//
// Created by ertls on 18.02.2026.
//

#include "ObjLoader.h"
#include <cstring>
#include "../../../Utils/FileIO.h"
#include <filesystem>
#include <unordered_map>
#include <charconv>

#include "../Geometry/Mesh.h"

using std::string;
using std::string_view;
using std::vector;
using std::cout;


namespace Obj::OBJLoader {

    OBJObject::OBJObject(const std::string &fileName) {
        namespace fs = std::filesystem;

        const auto filePath = fs::path("../Resources/Objects") / fileName;
        m_folderPath = filePath.parent_path().string() + "/";

        m_objFile = Utils::FileIO::readFile(filePath.string());

        m_glVertices.reserve(m_objFile.size());
        m_glUv.reserve(m_objFile.size());
        m_glNormals.reserve(m_objFile.size());

        m_allVertices.reserve(m_objFile.size());
        m_allUv.reserve(m_objFile.size());
        m_allNormals.reserve(m_objFile.size());
    }


    std::vector<Math::Vector3f>
    OBJObject::convertToVec3f(const std::vector<std::string_view>& lineList)
    {
        vector<Math::Vector3f> output;
        output.reserve(lineList.size());
        for (auto& line: lineList) {
            auto numberStrings = Utils::split(line, ' ');
            float nums[3] = {};
            for (int i = 0; auto numberString: numberStrings) {
                try {
                    nums[i++] = static_cast<float>(Utils::getDouble(numberString.data(), numberString.size()));
                } catch (...) {
                    cout << "invalid string caught: " << numberString << "\n";
                }
            }
            output.emplace_back(nums[0], nums[1], nums[2]);
        }
        return output;
    }

    std::vector<Math::Vector2f> OBJObject::convertToVec2f(const vector<string_view>& lineList) {
        vector<Math::Vector2f> output;
        output.reserve(lineList.size());
        for (auto& line: lineList) {
            auto numberStrings = Utils::split(line, ' ');
            float nums[2] = {};
            for (int i = 0; auto numberString: numberStrings) {
                try {
                    nums[i++] = static_cast<float>(Utils::getDouble(numberString.data(), numberString.size()));
                } catch (...) {
                    cout << "invalid string caught: " << numberString << "\n";
                }
            }
            output.emplace_back(nums[0], nums[1]);
        }
        return output;
    }

    void OBJObject::load() {
        m_lines = Utils::split(m_objFile, '\n') | std::views::filter([](auto s){return !s.empty() && !s.starts_with("#"); }) | Utils::to_vector;

        m_allVertices = convertToVec3f(getLinesWith("v "));
        m_allUv = convertToVec2f(getLinesWith("vt "));
        m_allNormals = convertToVec3f(getLinesWith("vn "));

        m_materialLib = getMaterialLib();

        loadMeshData();
        loadMaterial();
    }

    void OBJObject::loadMeshData() {
        std::unordered_map<IdxGroup, int, IdxGroupHash> map{};
        map.reserve(m_allVertices.size()*2);
        int i = 0;
        for (auto& face: getLinesWith("f ")) {
            auto idxGroups = loadIdxGroups(face);

            for (int numVerts = 0; auto& idxGroup: idxGroups) {
                if (numVerts > 3) {
                    m_indices.push_back(i-numVerts);
                    m_indices.push_back(i-1);
                }

                if (auto it = map.find(idxGroup); it != map.end()) {
                    m_indices.push_back(it->second);
                } else {
                    map.emplace(idxGroup, i);

                    m_glVertices.push_back(m_allVertices.at(idxGroup.v));
                    if (idxGroup.vt >= 0) m_glUv.push_back(m_allUv.at(idxGroup.vt));
                    else m_glUv.emplace_back(0.f, 0.f);

                    if (idxGroup.vn >= 0) m_glNormals.push_back(m_allNormals.at(idxGroup.vn));
                    else m_glNormals.emplace_back(0.f, 1.f, 0.f);

                    m_indices.push_back(i);
                    i++;
                }
                numVerts++;
            }
        }
    }

    std::vector<OBJObject::IdxGroup> OBJObject::loadIdxGroups(const std::string_view &faceLine) {
        vector<IdxGroup> output;
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

            output.emplace_back(v, vt, vn);
        }
        return output;
    }

    void OBJObject::loadMaterial() {
        if (m_materialLib.empty()) return;

        const auto fullPath = m_folderPath + std::string(m_materialLib);
        const auto s = Utils::FileIO::readFile(fullPath);

        for (const auto _lines = Utils::split(s, '\n');
            auto& line: _lines) {
            if (line.empty()) continue;
            if (line.starts_with("map_Kd ")) {
                m_textureName = line;
                m_textureName.erase(0, strlen("map_Kd "));
                break;
            }
        }
    }

    Mesh OBJObject::getMesh() {
        return {std::move(m_glVertices), std::move(m_glUv), std::move(m_glNormals), std::move(m_indices), Texture(m_folderPath + m_textureName)};
    }

    [[nodiscard]] vector<string_view> OBJObject::getLinesWith(const string_view &token) const {
        vector<string_view> output;

        for (auto& line: m_lines) {
            if (line.starts_with(token)) {
                auto newLine = line;
                newLine.remove_prefix(token.size());
                output.push_back(newLine);
            }
        }
        return output;
    }

    [[nodiscard]] string_view OBJObject::getMaterialLib() const {

        for (auto& line: m_lines) {
            if (line.starts_with("mtllib ")) {
                auto material = line;
                material.remove_prefix(strlen("mtllib "));
                return material;
            }
        }
        return {};
    }

    std::ostream& operator<<(std::ostream& os, const OBJObject& o) {
        os << "OBJObject:\n";
        os << "  Material lib: " << o.m_materialLib << "\n";
        // os << "  Vertices:     " << o.vertexLines.size() << "\n";
        // os << "  Texcoords:    " << o.textureLines.size() << "\n";
        // os << "  Normals:      " << o.normalsLines.size() << "\n";
        // os << "  Faces:        " << o.facesLines.size() << "\n";
        return os;
    }

} // Loader
// Obj