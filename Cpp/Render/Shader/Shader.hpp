//
// Created by ertls on 17.02.2026.
//

#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <unordered_map>

#include "../../Utils/Math/Matrix.hpp"
#include "glad/gl.h"

namespace Render {
    class Shader {
    public:
        explicit Shader(std::filesystem::path&& fileVert, std::filesystem::path&& fileFrag);

        [[nodiscard]] GLuint createVertexShader(const std::string &shaderSource) const;

        static std::string readShaderFile(const std::filesystem::path& fileName);

        [[nodiscard]] GLuint createFragmentShader(const std::string &shaderSource) const;

        enum class UniformType {
            Int,
            Float,
            Bool,

            Vec2,
            Vec3,
            Vec4,

            Mat3,
            Mat4,

            Sampler2D,
            SamplerCube,

            Unknown
        };

        GLuint createShader(const GLchar *code, int shaderType) const;

        void bind() const;
        void unbind() const;

        virtual ~Shader();

        static void typeCheck(UniformType a, UniformType b);

        void setUniform(const std::string& name, float value) const;
        void setUniform(const std::string& name, int value) const;
        void setUniform(const std::string& name, const  ggm::Vector3f& value) const;
        void setUniform(const std::string& name, const ggm::Matrix4f& value) const;

        void createUniform(const std::string &name, UniformType type);
        void createUniform(const std::string &name);

        virtual void compile();
        void link() const;

        struct ShaderException : std::runtime_error{
            explicit ShaderException(const std::string &msg);
        };

    private:
        struct Uniform {
            GLint location;
            UniformType type;
        };

        std::filesystem::path mVertPath{};
        std::filesystem::path mFragPath{};
        std::unordered_map <std::string, Uniform> mUniformLocationMap{};
        GLuint mProgramId = 0;
        GLuint mVertexId = 0;
        GLuint mFragmentId = 0;

        struct GLUniform {
            std::string_view name;
            UniformType type;

            GLUniform(std::string_view type, std::string_view name);
        };

        static std::vector<GLUniform>
            getUniforms(const std::string& shaderFile);

        const Uniform& getUniform(const std::string &name) const;
    };
    std::ostream& operator<<(std::ostream& lhs, Shader::UniformType rhs);
}