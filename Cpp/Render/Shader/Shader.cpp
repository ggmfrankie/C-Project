//
// Created by ertls on 17.02.2026.
//

#include "Shader.h"

#include <utility>

#include "../../Utils/FileIO.h"
#include "glad/gl.h"

namespace Render {
    Shader::Shader(std::string  fileVert, std::string  fileFrag): vertPath(std::move(fileVert)), fragPath(std::move(fileFrag)) {
        programId = 0;
        vertexId = 0;
        fragmentId = 0;
    }

    Shader::~Shader() {
        glDeleteProgram(programId);
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
    }

    void Shader::link() const {
        glLinkProgram(programId);
        glValidateProgram(programId);
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

    GLuint Shader::createShader(const GLchar *code, int shaderType) const {
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




