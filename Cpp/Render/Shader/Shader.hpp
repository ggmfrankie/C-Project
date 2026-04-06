//
// Created by ertls on 17.02.2026.
//

#ifndef MIXEDPROJECT_SHADER_H
#define MIXEDPROJECT_SHADER_H
#include <string>
#include <unordered_map>

#include "../../Utils/Math/Matrix.hpp"
#include "glad/gl.h"

namespace Render {
    class Shader {
    public:
        explicit Shader(std::string fileVert, std::string fileFrag);

        [[nodiscard]] GLuint createVertexShader(const std::string &fileVert) const;

        static std::string readShaderFile(const std::string &fileName);

        [[nodiscard]] GLuint createFragmentShader(const std::string &fileFrag) const;

        GLuint createShader(const GLchar *code, int shaderType) const;

        void bind() const;
        void unbind() const;

        virtual ~Shader();

        void setUniform(const std::string &name, float value) const;
        void setUniform(const std::string &name, int value) const;
        void setUniform(const std::string &name, const  ggm::Vector3f& value) const;
        void setUniform(const std::string &name, const ggm::Matrix4f &value) const;

        void createUniform(const std::string &name);
        GLint getUniformLocation(const std::string &name) const;

        virtual void compile();
        void link() const;

    private:
        std::string vertPath{};
        std::string fragPath{};
        std::unordered_map <std::string, GLint> uniformLocationMap{};
        GLuint programId = 0;
        GLuint vertexId = 0;
        GLuint fragmentId = 0;

    };
}


#endif //MIXEDPROJECT_SHADER_H