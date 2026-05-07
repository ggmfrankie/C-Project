//
// Created by ertls on 17.02.2026.
//

#include "Shader.hpp"
#include "Shader.hpp"

#include <cassert>
#include <cstring>
#include <utility>

#include "../../Utils/FileIO.hpp"
#include "../../Utils/Math/Matrix.hpp"
#include "../../Utils/Math/Vector.hpp"
#include "glad/gl.h"
#include "Utils/Utils.hpp"
#include "Utils/DataStorage/LazyStream.hpp"

namespace Render {
    using std::string;
    using std::filesystem::path;
    Shader::Shader(path&& fileVert, path&& fileFrag):
        mVertPath(std::move(fileVert)),
        mFragPath(std::move(fileFrag))
    {}

    Shader::~Shader() {
        glDeleteProgram(mProgramId);
    }

    void Shader::typeCheck(UniformType a, UniformType b) {
        return;
        if (a == UniformType::Unknown || b == UniformType::Unknown) return;
        if (a == b) return;
        std::ostringstream error;
        error << "[Warning] Uniform type '" << a << "' not matching with Uniform type '" << b << "' in shader program.\n";
        throw ShaderException(error.str());
    }

    void Shader::setUniform(const string &name, const float value) const {
        auto[loc, type] = getUniform(name);
        typeCheck(type, UniformType::Float);
        glUniform1f(loc, value);
    }

    void Shader::setUniform(const string &name, const int value) const {
        auto[loc, type] = getUniform(name);
        typeCheck(type, UniformType::Int);
        glUniform1i(loc, value);
    }

    void Shader::setUniform(const string &name, const ggm::Vector3f& value) const {
        auto[loc, type] = getUniform(name);
        typeCheck(type, UniformType::Vec3);
        glUniform3f(loc, value.x, value.y, value.z);
    }

    void Shader::setUniform(const string &name, const ggm::Matrix4f& value) const {
        auto[loc, type] = getUniform(name);
        typeCheck(type, UniformType::Mat4);
        glUniformMatrix4fv(loc,1, false, value.getDataPtr());
    }

    void Shader::createUniform(const string &name, UniformType type) {
        const GLint loc = glGetUniformLocation(mProgramId, name.c_str());
        if (loc == -1) {
            std::ostringstream error;
            error << "[Warning] Uniform '" << name << "' not found in shader program.\n";
            throw ShaderException(error.str());
        }
        mUniformLocationMap.try_emplace(name, Uniform{loc, type});
    }

    void Shader::createUniform(const string &name) {
       createUniform(name, UniformType::Unknown);
    }

    const Shader::Uniform& Shader::getUniform(const std::string& name) const {
        const auto it = mUniformLocationMap.find(name);
        if (it == mUniformLocationMap.end()) {
            std::ostringstream error;
            error << "[Error] Uniform '" << name << "' was not created via Shader::createUniform.\n";
            throw ShaderException(error.str());
        }
        return it->second;
    }

    Shader::GLUniform::GLUniform(std::string_view type, std::string_view name) :
        name(name),
        type([](auto v) {
            using namespace std::literals;
            using std::pair;
            constexpr std::array pairs{
                pair{"int"sv, UniformType::Int},
                pair{"float"sv, UniformType::Float},
                pair{"bool"sv, UniformType::Bool},
                pair{"vec2"sv, UniformType::Vec2},
                pair{"vec3"sv, UniformType::Vec3},
                pair{"vec4"sv, UniformType::Vec4},
                pair{"mat3"sv, UniformType::Mat3},
                pair{"mat4"sv, UniformType::Mat4},
                pair{"sampler2D"sv, UniformType::Sampler2D},
                pair{"samplerCube"sv, UniformType::SamplerCube}
            };

            for (auto[V, T]: pairs) {
                if (v == V) return T;
            }

            return UniformType::Unknown;
        }(type))
    {}

    std::vector<Shader::GLUniform> Shader::getUniforms(const std::string& shaderFile) {
        using sv = const std::string_view;
        auto out = ggm::LazyStream(ggm::split(shaderFile, '\n'))
            .filter([](auto& a){ return a.starts_with("uniform ");})
            .map([](auto a) {
                a.remove_prefix(strlen("uniform "));
                size_t pos = a.find_first_of(" ");
                sv type = a.substr(0, pos);
                sv name = a.substr(pos+1);

                return GLUniform{type, name};
            })
            .toVector();

        return out;
    }

    void Shader::compile() {
        mProgramId = glCreateProgram();
        int success;
        char infoLog[512];

        const std::string vertShader = readShaderFile(mVertPath);
        const std::string fragShader = readShaderFile(mFragPath);

        mVertexId = createVertexShader(vertShader);
        mFragmentId = createFragmentShader(fragShader);

        glGetShaderiv(mVertexId, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(mVertexId, 511, nullptr, infoLog);
            std::ostringstream error;
            error << "Vertex Shader Compile Error:\n" << infoLog << "\n";
            throw ShaderException(error.str());
        }

        glGetShaderiv(mFragmentId, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(mFragmentId, 511, nullptr, infoLog);
            std::ostringstream error;
            error << "Fragment Shader Compile Error:\n" << infoLog << "\n";
            throw ShaderException(error.str());
        }

        auto uniformsV = getUniforms(vertShader);
        auto uniformsF = getUniforms(fragShader);

        for (auto[name, type]: uniformsV) {
            createUniform(string(name), type);
        }
    }

    void Shader::link() const {
        int success;
        char infoLog[512];
        glLinkProgram(mProgramId);

        glGetProgramiv(mProgramId, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(mProgramId, 512, nullptr, infoLog);
            std::ostringstream error;
            error << "Shader Program Link Error:\n" << infoLog << "\n";
            throw ShaderException(error.str());
        }

        glValidateProgram(mProgramId);
    }

    GLuint Shader::createVertexShader(const std::string &shaderSource) const {
        const GLuint shaderId = createShader(shaderSource.c_str(), GL_VERTEX_SHADER);
        return shaderId;
    }

    GLuint Shader::createFragmentShader(const std::string &shaderSource) const {
        const GLuint shaderId = createShader(shaderSource.c_str(), GL_FRAGMENT_SHADER);
        return shaderId;
    }

    std::string Shader::readShaderFile(const path& fileName) {
        return ggm::FileIO::readFile("../Cpp/Render/Shader/GlslShader" / fileName);
    }

    GLuint Shader::createShader(const GLchar *code, const int shaderType) const {
        const GLuint shaderId = glCreateShader(shaderType);

        const GLchar* codePtr = code;
        glShaderSource(shaderId, 1, &codePtr, nullptr);

        glCompileShader(shaderId);

        glAttachShader(mProgramId, shaderId);
        return shaderId;
    }

    void Shader::bind() const {
        glUseProgram(mProgramId);
    }

    void Shader::unbind() const {
        glUseProgram(0);
    }

    Shader::ShaderException::ShaderException(const std::string &msg) :
        runtime_error(msg)
    {}

    constexpr std::string_view
    uniformTypeToString(Shader::UniformType type)
    {
        using enum Shader::UniformType;

        switch (type) {
            case Int:           return "int";
            case Float:         return "float";
            case Bool:          return "bool";

            case Vec2:          return "vec2";
            case Vec3:          return "vec3";
            case Vec4:          return "vec4";

            case Mat3:          return "mat3";
            case Mat4:          return "mat4";

            case Sampler2D:     return "sampler2D";
            case SamplerCube:   return "samplerCube";

            default:            return "unknown";
        }
    }

    std::ostream& operator<<(std::ostream& lhs, Shader::UniformType rhs) {
        lhs << uniformTypeToString(rhs);
        return lhs;
    }
}





