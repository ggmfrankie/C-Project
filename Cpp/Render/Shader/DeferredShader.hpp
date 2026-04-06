//
// Created by ertls on 11.03.2026.
//

#ifndef MIXEDPROJECT_DEFERREDSHADER_HPP
#define MIXEDPROJECT_DEFERREDSHADER_HPP
#include "Shader.hpp"

namespace Render {
    class DeferredShader: public Shader {
        GLuint gBufferId = 0;

    public:
        DeferredShader(const std::string&  fileVert, const std::string&  fileFrag);
        void compile() override;
    };
} // Render

#endif //MIXEDPROJECT_DEFERREDSHADER_HPP