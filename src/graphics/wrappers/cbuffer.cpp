
#include "CBuffer.h"

#include <GL/glew.h>
#include <iostream>
#include "types.h"
#include "error.h"

CBuffer::CBuffer(int width, int height) : _width(width), _height(height) {

}

CBuffer::~CBuffer() {

}

void CBuffer::init() {

    // Create the FBO
    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

    // Create the CBuffer textures
    glGenTextures(CBUFFER_NUM_TEXTURES, _textures);

    std::cout << "Texture Ambient: " << _textures[CBUFFER_TEXTURE_TYPE_AMBIENT] << std::endl;
    std::cout << "Texture Diffuse: " << _textures[CBUFFER_TEXTURE_TYPE_DIFFUSE] << std::endl;
    std::cout << "Texture Specular: " << _textures[CBUFFER_TEXTURE_TYPE_SPECULAR] << std::endl;

    // Initialize "color" textures
    for (unsigned int i = 0; i < CBUFFER_NUM_TEXTURES; i++) {
        glBindTexture(GL_TEXTURE_2D, _textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _width, _height, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _textures[i], 0);
    }


    // Set up the draw buffers
    GLenum draw_buffers[CBUFFER_NUM_TEXTURES];
    for (int i = 0; i < CBUFFER_NUM_TEXTURES; i++) {
        draw_buffers[i] = GL_COLOR_ATTACHMENT0 + i;
    }
    glDrawBuffers(CBUFFER_NUM_TEXTURES, draw_buffers);

    // Check for errors    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "FB error, status: " << status << std::endl;
        abort();
    }

    // restore default FBO
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glCheckError();
    return;

}

void CBuffer::use() {

    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glViewport(0, 0, _width, _height);
    glCheckError();
}

void CBuffer::unuse() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glCheckError();
}

void CBuffer::read(std::shared_ptr<Shader> shader, size_t pos) {
    // Ambient
    shader->setTextureUniform("unTexAmbient", pos);
    glActiveTexture(GL_TEXTURE0 + pos);
    glBindTexture(GL_TEXTURE_2D, _textures[CBUFFER_TEXTURE_TYPE_AMBIENT]);

    // Diffuse
    shader->setTextureUniform("unTexDiffuse", pos+1);
    glActiveTexture(GL_TEXTURE1 + pos);
    glBindTexture(GL_TEXTURE_2D, _textures[CBUFFER_TEXTURE_TYPE_DIFFUSE]);

    // Specular
    shader->setTextureUniform("unTexSpecular", pos+2);
    glActiveTexture(GL_TEXTURE2 + pos);
    glBindTexture(GL_TEXTURE_2D, _textures[CBUFFER_TEXTURE_TYPE_SPECULAR]);

    m_pos = pos;

}

void CBuffer::unread() {
    for (int i = 0; i < CBUFFER_NUM_TEXTURES; i++) {
        glActiveTexture(GL_TEXTURE0 + m_pos + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void CBuffer::dispose() {

    glDeleteFramebuffers(1, &_fbo);

    glDeleteTextures(CBUFFER_NUM_TEXTURES, _textures);
    glCheckError();
}
