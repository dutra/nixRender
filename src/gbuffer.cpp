
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
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

    // Create the gbuffer textures
    glGenTextures(GBUFFER_NUM_TEXTURES, _textures);
    glGenTextures(1, &_depth_texture);

    std::cout << "Texture Position: " << _textures[GBUFFER_TEXTURE_TYPE_POSITION] << std::endl;
    std::cout << "Texture Normal: " << _textures[GBUFFER_TEXTURE_TYPE_NORMAL] << std::endl;
    std::cout << "Texture Diffuse: " << _textures[GBUFFER_TEXTURE_TYPE_DIFFUSE] << std::endl;
    std::cout << "Texture Texcoord: " << _textures[GBUFFER_TEXTURE_TYPE_TEXCOORD] << std::endl;
    std::cout << "Texture Depth: " << _depth_texture << std::endl;


    // Initialize "color" textures
    for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
        glBindTexture(GL_TEXTURE_2D, _textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, _width, _height, 0, GL_RGB, GL_FLOAT, nullptr);
       	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _textures[i], 0);
    }
    
    // Initialize depth texture
    glBindTexture(GL_TEXTURE_2D, _depth_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depth_texture, 0);

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
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
    
    glCheckError();
}

void GBuffer::read(std::shared_ptr<Shader> shader) {
    // position
    shader->setTextureUniform("positionTex", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _textures[GBUFFER_TEXTURE_TYPE_POSITION]);
    
    // normal
    shader->setTextureUniform("normalTex", 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _textures[GBUFFER_TEXTURE_TYPE_NORMAL]);

    // diffuse
    shader->setTextureUniform("diffuseTex", 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _textures[GBUFFER_TEXTURE_TYPE_DIFFUSE]);
    
}

void GBuffer::unread() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GBuffer::dispose() {
    
    glDeleteFramebuffers(1, &_fbo);
    
    glDeleteTextures(GBUFFER_NUM_TEXTURES, _textures);
    glDeleteTextures(1, &_depth_texture);
    glCheckError();
}