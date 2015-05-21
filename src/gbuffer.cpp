
#include "gbuffer.h"

#include <GL/glew.h>
#include <iostream>
#include "types.h"
#include "error.h"

GBuffer::GBuffer(int width, int height) : _width(width), _height(height) {

}

GBuffer::~GBuffer() {

}

void GBuffer::init() {

    // Create the FBO
    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);

    // Create the gbuffer textures
    glGenTextures(GBUFFER_NUM_TEXTURES, _textures);
    glGenTextures(1, &_depth_texture);

    std::cout << "Texture Position: " << _textures[0] << std::endl;
    std::cout << "Texture Diffuse: " << _textures[1] << std::endl;
    std::cout << "Texture Normal: " << _textures[2] << std::endl;
    std::cout << "Texture Texcoord: " << _textures[3] << std::endl;
    std::cout << "Texture Depth: " << _depth_texture << std::endl;


    // Initialize "color" textures
    for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
        glBindTexture(GL_TEXTURE_2D, _textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, _width, _height, 0, GL_RGB, GL_FLOAT, nullptr);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _textures[i], 0);
    }
    
    // Initialize depth texture
    glBindTexture(GL_TEXTURE_2D, _depth_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depth_texture, 0);

    // Set up the draw buffers
    GLenum draw_buffers[GBUFFER_NUM_TEXTURES] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
    glDrawBuffers(GBUFFER_NUM_TEXTURES, draw_buffers);

    // Check for errors    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "FB error, status: " << status << std::endl;
        abort();
    }

    // restore default FBO
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glCheckError();
    return;

}

void GBuffer::use() {

    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glViewport(0, 0, _width, _height);
    glCheckError();
}

void GBuffer::unuse() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, _width, _height);
    glCheckError();
}

void GBuffer::dispose() {

    glDeleteFramebuffers(1, &_fbo);
    
    glDeleteTextures(GBUFFER_NUM_TEXTURES, _textures);
    glDeleteTextures(1, &_depth_texture);
    glCheckError();
}