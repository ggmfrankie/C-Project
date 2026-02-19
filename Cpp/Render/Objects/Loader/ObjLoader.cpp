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
        folderPath = filePath.parent_path().string() + "/";

        objFile = Utils::FileIO::readFile(filePath.string());

        glVertices.reserve(objFile.size());
        glUv.reserve(objFile.size());
        glNormals.reserve(objFile.size());
    }

    auto OBJObject::convertToVec3f(const vector<string_view>& lineList) {
        vector<Math::Vector3f> output;
        output.reserve(lineList.size());
        for (auto& line: lineList) {
            auto numberStrings = Utils::split(line, ' ');
            float nums[3] = {};
            for (int i = 0; auto numberString: numberStrings) {
                try {
                    nums[i++] = std::stof(string(numberString));
                } catch (...) {
                    cout << "invalid string caught: " <<numberString << "\n";
                }
            }
            output.emplace_back(nums[0], nums[1], nums[2]);
        }
        return output;
    }

    auto OBJObject::convertToVec2f(const vector<string_view>& lineList) {
        vector<Math::Vector2f> output;
        output.reserve(lineList.size());
        for (auto& line: lineList) {
            auto numberStrings = Utils::split(line, ' ');
            float nums[2] = {};
            for (int i = 0; auto numberString: numberStrings) {
                try {
                    nums[i++] = std::stof(string(numberString));
                } catch (...) {
                    cout << "invalid string caught: " <<numberString << "\n";
                }
            }
            output.emplace_back(nums[0], nums[1]);
        }
        return output;
    }

    void OBJObject::load() {
        lines = Utils::split(objFile, '\n') | std::views::filter([](auto s){return !s.empty() && !s.starts_with("#"); }) | Utils::to_vector;

        materialLib = getMaterialLib();

        allVertices = convertToVec3f(getLinesWith("v "));
        allUv = convertToVec2f(getLinesWith("vt "));
        allNormals = convertToVec3f(getLinesWith("vn "));

        std::unordered_map<Face::IdxGroup, int, Face::IdxGroupHash> map{};
        map.reserve(allVertices.size()*2);

        for (auto& face: getLinesWith("f ")) {
            faces.emplace_back(face);

            for (int i = 0; auto& idxGroup: faces.back().getIdxGroups()) {
                if (map.contains(idxGroup)) {
                    indices.push_back(map.at(idxGroup));
                } else {
                    map.emplace(idxGroup, i);

                    glVertices.push_back(allVertices[idxGroup.v]);
                    glUv.push_back(allUv[idxGroup.vt]);
                    glNormals.push_back(allNormals[idxGroup.vn]);

                    indices.push_back(i);
                }
                i++;
            }
        }
    }

    Mesh OBJObject::getMesh() {
        return {std::move(glVertices), std::move(glUv), std::move(glNormals), std::move(indices)};
    }

    [[nodiscard]] vector<string_view> OBJObject::getLinesWith(const string_view &token) const {
        vector<string_view> output;

        for (auto& line: lines) {
            if (line.starts_with(token)) {
                auto newLine = line;
                newLine.remove_prefix(strlen(token.data()));
                output.push_back(newLine);
            }
        }
        return output;
    }

    [[nodiscard]] string_view OBJObject::getMaterialLib() const {

        for (auto& line: lines) {
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
        os << "  Material lib: " << o.materialLib << "\n";
        // os << "  Vertices:     " << o.vertexLines.size() << "\n";
        // os << "  Texcoords:    " << o.textureLines.size() << "\n";
        // os << "  Normals:      " << o.normalsLines.size() << "\n";
        // os << "  Faces:        " << o.facesLines.size() << "\n";
        return os;
    }

} // Loader
// Obj