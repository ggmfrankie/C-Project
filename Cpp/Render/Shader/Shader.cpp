//
// Created by ertls on 17.02.2026.
//

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

    void Shader::createUniform(const string &name, UniformType type) {
        const GLint loc = glGetUniformLocation(mProgramId, name.c_str());
        if (loc == -1) {
            std::ostringstream error;
            error << "[Warning] Uniform '" << name << "' not found in shader program.\n";
            throw ShaderException(error.str());
        }
        mUniformLocationMap.try_emplace(name, Uniform{loc, type});
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
            using enum UniformType;
            using std::pair;
            constexpr std::array pairs{
                pair{"int"sv, Int},
                pair{"float"sv, Float},
                pair{"bool"sv, Bool},
                pair{"vec2"sv, Vec2},
                pair{"vec3"sv, Vec3},
                pair{"vec4"sv, Vec4},
                pair{"mat3"sv, Mat3},
                pair{"mat4"sv, Mat4},
                pair{"sampler2D"sv, Sampler2D},
                pair{"samplerCube"sv, SamplerCube}
            };

            for (auto[V, T]: pairs) {
                if (v == V) return T;
            }

            return UniformType::Unknown;
        }(type))
    {}

    std::vector<Shader::GLUniform>
    Shader::getUniforms(const std::string& shaderFile) {
        using sv = const std::string_view;
        auto out = ggm::LazyStream(ggm::split(shaderFile, '\n'))
            .filter([](auto& a){ return a.starts_with("uniform ");})
            .map([](auto a) {
                a.remove_prefix(strlen("uniform "));
                size_t pos = a.find_first_of(" ");
                sv type = a.substr(0, pos);
                const size_t end = a.find_first_of(";");
                sv name = a.substr(pos + 1, end - (pos + 1));

                return GLUniform{type, name};
            })
            .toVector();

        return out;
    }

    void Shader::compileAndLink() {
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

        glLinkProgram(mProgramId);

        glGetProgramiv(mProgramId, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(mProgramId, 512, nullptr, infoLog);
            std::ostringstream error;
            error << "Shader Program Link Error:\n" << infoLog << "\n";
            throw ShaderException(error.str());
        }

        glValidateProgram(mProgramId);

        auto uniformsV = getUniforms(vertShader);
        auto uniformsF = getUniforms(fragShader);

        for (auto[name, type]: uniformsV) {
            createUniform(string(name), type);
        }

        for (auto[name, type]: uniformsF) {
            createUniform(string(name), type);
        }
    }

    GLuint Shader::createVertexShader(const std::string &shaderSource) const {
        return  createShader(shaderSource.c_str(), GL_VERTEX_SHADER);
    }

    GLuint Shader::createFragmentShader(const std::string &shaderSource) const {
        return createShader(shaderSource.c_str(), GL_FRAGMENT_SHADER);
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

    // ReSharper disable once CppMemberFunctionMayBeStatic
    void Shader::unbind() const { // NOLINT(*-convert-member-functions-to-static)
        glUseProgram(0);
    }

    Shader::ShaderException::ShaderException(const std::string &msg) :
        runtime_error(msg)
    {}

    constexpr std::string_view
    uniformTypeToString(UniformType type)
    {
        using enum UniformType;

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

    std::ostream& operator<<(std::ostream& lhs, UniformType rhs) {
        lhs << uniformTypeToString(rhs);
        return lhs;
    }
}





