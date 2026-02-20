//
// Created by ertls on 17.02.2026.
//

#include "Shader.h"

#include <iostream>
#include <utility>

#include "../../Utils/FileIO.h"
#include "../../Utils/Math/Matrix.h"
#include "../../Utils/Math/Vector.h"
#include "glad/gl.h"

namespace Render {
    using std::string;
    Shader::Shader(string  fileVert, string  fileFrag): vertPath(std::move(fileVert)), fragPath(std::move(fileFrag)) {
        programId = 0;
        vertexId = 0;
        fragmentId = 0;
    }

    Shader::~Shader() {
        glDeleteProgram(programId);
    }

    void Shader::setUniform(const string &name, const float value) const {
        glUniform1f(getUniformLocation(name), value);
    }

    void Shader::setUniform(const string &name, const int value) const {
        glUniform1i(getUniformLocation(name), value);
    }

    void Shader::setUniform(const string &name, const Math::Vector3f& value) const {
        glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
    }

    void Shader::setUniform(const string &name, const Math::Matrix4f& value) const {
        glUniformMatrix4fv(getUniformLocation(name),1, false, value.getDataPtr());
    }

    void Shader::createUniform(const string &name) {
        const GLint loc = glGetUniformLocation(programId, name.c_str());
        if (loc == -1) {
            std::cerr << "[Warning] Uniform '" << name << "' not found in shader program.\n";
        }
        uniformLocationMap[name] = loc;
    }

    GLint Shader::getUniformLocation(const std::string& name) const {
        const auto it = uniformLocationMap.find(name);
        if (it == uniformLocationMap.end()) {
            std::cerr << "[Error] Uniform '" << name
                      << "' was not created via Shader::createUniform.\n";
            return -1;
        }
        return it->second;
    }

    void Shader::init() {
        programId = glCreateProgram();
        int success;
        char infoLog[512];

        vertexId = createVertexShader(vertPath);
        fragmentId = createFragmentShader(fragPath);

        glGetShaderiv(vertexId, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexId, 512, nullptr, infoLog);
            printf("Vertex Shader Compile Error:\n%s\n", infoLog);
        }

        // --- DEBUG: Check fragment shader compile status ---
        glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentId, 512, nullptr, infoLog);
            printf("Fragment Shader Compile Error:\n%s\n", infoLog);
        }

        glLinkProgram(programId);

        // --- DEBUG: Check program link status ---
        glGetProgramiv(programId, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(programId, 512, nullptr, infoLog);
            printf("Shader Program Link Error:\n%s\n", infoLog);
        } else {
            printf("Shader Program linked successfully! ID: %d\n", programId);
        }
        vertPath = {};
        fragPath = {};
    }

    void Shader::link() const {
        glLinkProgram(programId);
        glValidateProgram(programId);
        puts("linked shader program");
    }

    GLuint Shader::createVertexShader(const std::string &fileVert) const {
        const std::string shaderSource = readShaderFile(fileVert);

        const GLuint shaderId = createShader(shaderSource.c_str(), GL_VERTEX_SHADER);

        return shaderId;
    }

    GLuint Shader::createFragmentShader(const std::string &fileFrag) const {
        const std::string shaderSource = readShaderFile(fileFrag);

        const GLuint shaderId = createShader(shaderSource.c_str(), GL_FRAGMENT_SHADER);

        return shaderId;
    }

    std::string Shader::readShaderFile(const std::string& fileName) {
        return Utils::FileIO::readFile("../Cpp/Render/Shader/GlslShader/" + fileName);
    }

    GLuint Shader::createShader(const GLchar *code, const int shaderType) const {
        const GLuint shaderId = glCreateShader(shaderType);

        const GLchar* codePtr = code;    // pointer to C-string
        glShaderSource(shaderId, 1, &codePtr, nullptr);

        glCompileShader(shaderId);

        glAttachShader(programId, shaderId);
        return shaderId;
    }

    void Shader::bind() const {
        glUseProgram(programId);
    }

    void Shader::unbind() {
        glUseProgram(0);
    }

}




