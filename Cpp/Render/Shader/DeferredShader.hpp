//
// Created by ertls on 11.03.2026.
//

#pragma once
#include "../EngineDefines.hpp"
#include "Shader.hpp"

namespace Render {
    class DeferredShader final : public Shader {
        GLuint gBufferId = 0;

    public:
        DeferredShader(const std::string&  fileVert, const std::string&  fileFrag);
        void compileAndLink() override;
    };
} // Render
