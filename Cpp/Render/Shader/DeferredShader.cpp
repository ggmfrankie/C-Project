//
// Created by ertls on 11.03.2026.
//

#include "DeferredShader.hpp"

namespace Render {
    DeferredShader::DeferredShader(const std::string &fileVert, const std::string &fileFrag) : Shader(fileVert, fileFrag) {

    }

    void DeferredShader::compile() {
        constexpr GLuint screenWidth = 1024, screenHeight = 768;

        Shader::compile();
        glGenFramebuffers(1, &gBufferId);
        glBindFramebuffer(GL_FRAMEBUFFER, gBufferId);

        GLuint gPos, gNorm, gColor;

        glGenTextures(1, &gPos);
        glBindTexture(GL_TEXTURE_2D, gPos);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPos, 0);

        glGenTextures(1, &gNorm);
        glBindTexture(GL_TEXTURE_2D, gNorm);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNorm, 0);

        glGenTextures(1, &gColor);
        glBindTexture(GL_TEXTURE_2D, gColor);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColor, 0);

        unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
        glDrawBuffers(3, attachments);
    }
} // Render