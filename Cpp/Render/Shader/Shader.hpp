//
// Created by ertls on 17.02.2026.
//

#pragma once
#include "../EngineDefines.hpp"
#include <filesystem>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

#include "../../Utils/Math/Matrix.hpp"
#include "glad/gl.h"

namespace Render {
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
    std::ostream& operator<<(std::ostream& lhs, UniformType rhs);

    class Shader {
    public:
        explicit Shader(std::filesystem::path&& fileVert, std::filesystem::path&& fileFrag);

        [[nodiscard]] GLuint createVertexShader(const std::string &shaderSource) const;

        static std::string readShaderFile(const std::filesystem::path& fileName);

        [[nodiscard]] GLuint createFragmentShader(const std::string &shaderSource) const;

        GLuint createShader(const GLchar *code, int shaderType) const;

        void bind() const;
        void unbind() const;

        virtual ~Shader();
        virtual void compileAndLink();

        template <size_t N>
        struct ConstStr {
            char value[N]{};

            explicit consteval ConstStr(const char (&str)[N]) {
                std::copy_n(str, N, value);
            }
        };

        template <size_t N>
        static consteval uint32_t fnv1a(const char (&s)[N]) {
            uint32_t hash = 2166136261u;
            for (size_t i = 0; i < N - 1; ++i) {
                hash ^= static_cast<uint32_t>(s[i]);
                hash *= 16777619u;
            }
            return hash;
        }

        /// @brief Sets the specified uniform to the value.
        /// Extracts uniform location once and caches it inside a static variable. Each invocation generates a unique instance.
        /// Does typechecking once and throws an error if types don't match.

#define setUniform(Name, Value) setUniform_T<Render::Shader::ConstStr{Name}, Render::Shader::fnv1a(__FILE__)>(Value)


        template<ConstStr Name, uint32_t ID, typename V>
        void setUniform_T(V value) {
            using enum UniformType;
            const static Uniform u = getUniform(Name.value);
            const static auto typeCheck = [](UniformType a, UniformType b) {
                if (a == Unknown || b == Unknown || a == b) return true;
                std::ostringstream error;
                error << "[Warning] Uniform type '" << a << "' not matching with Uniform type '" << b << "' in shader program.\n";
                throw ShaderException(error.str());
            };

            if constexpr (std::is_same_v<V, float>) {
                static bool _ = typeCheck(u.type, Float);
                glUniform1f(u.location, value);
            } else if constexpr (std::is_same_v<V, int>) {
                static bool _ = typeCheck(u.type, Int);
                glUniform1i(u.location, value);
            } else if constexpr (std::is_same_v<V, bool>) {
                static bool _ = typeCheck(u.type, Bool);
                glUniform1i(u.location, value ? 1 : 0);
            } else if constexpr (std::is_same_v<V, ggm::Vector2f>) {
                static bool _ = typeCheck(u.type, Vec2);
                glUniform2f(u.location, value.x, value.y);
            } else if constexpr (std::is_same_v<V, ggm::Vector3f>) {
                static bool _ = typeCheck(u.type, Vec3);
                glUniform3f(u.location, value.x, value.y, value.z);
            } else if constexpr (std::is_same_v<V, ggm::Vector4f>) {
                static bool _ = typeCheck(u.type, Vec4);
                static_assert(false, "Unsupported uniform type");
            } else if constexpr (std::is_same_v<V, ggm::Matrix3f>) {
                static bool _ = typeCheck(u.type, Mat3);
                static_assert(false, "Unsupported uniform type");
            } else if constexpr (std::is_same_v<V, ggm::Matrix4f>) {
                static bool _ = typeCheck(u.type, Mat4);
                glUniformMatrix4fv(u.location,1, false, value.getDataPtr());
            } else {
                static_assert(false, "Unsupported uniform type");
            }
        }

        struct ShaderException final : std::runtime_error{
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
        void createUniform(const std::string &name, UniformType type);
    };

}