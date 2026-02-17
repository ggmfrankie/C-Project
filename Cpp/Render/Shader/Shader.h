//
// Created by ertls on 17.02.2026.
//

#ifndef MIXEDPROJECT_SHADER_H
#define MIXEDPROJECT_SHADER_H
#include <string>

#include "glad/gl.h"

namespace Render{
    class Shader {
    public:
        explicit Shader(std::string  fileVert, std::string  fileFrag);

        [[nodiscard]] GLuint createVertexShader(const std::string &fileVert) const;

        static std::string readShaderFile(const std::string &fileName);

        [[nodiscard]] GLuint createFragmentShader(const std::string &fileFrag) const;

        GLuint createShader(const GLchar *code, int shaderType) const;

        void bind() const;

        static void unbind();

        ~Shader();

        void init();

        void link() const;

    private:
        std::string vertPath;
        std::string fragPath;
        GLuint programId;
        GLuint vertexId;
        GLuint fragmentId;
    };
}


#endif //MIXEDPROJECT_SHADER_H