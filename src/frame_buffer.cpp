#include "frame_buffer.h"

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

FrameBuffer::FrameBuffer(int window_width, int window_height)
                                                                                    : _width(window_width), _height(window_height) {

}

void FrameBuffer::init() {
    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

    // Create texture to hold color buffer
    glGenTextures(1, &_texture_color);
    glBindTexture(GL_TEXTURE_2D, _texture_color);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Attach the texture to the color buffer in our frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture_color, 0);

    // Set the output location for pixels
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    // Unbind used resources
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
/*
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _fbo_depth); // Attach the depth buffer fbo_depth to our frame buffer

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER); // Check that status of our generated frame buffer

    if (status != GL_FRAMEBUFFER_COMPLETE) { // If the frame buffer does not report back as complete
        std::cout << red << "ERROR: Couldn't create frame buffer" << std::endl; // Make sure you include <iostream>
        exit(0); // Exit the application
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
*/
}

void FrameBuffer::initDepth() {
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

    // generate depth texture
    glGenTextures(1, &_texture_depth);
    glBindTexture(GL_TEXTURE_2D, _texture_depth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _width, _height, 0, GL_RED, GL_FLOAT, nullptr);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _texture_depth, 0);

    // Unbind used resources
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer() {
    if (_fbo != 0) {
        glDeleteFramebuffers(1, &_fbo);
        _fbo = 0;
    }
    if (_texture_color != 0) {
        glDeleteTextures(1, &_texture_color);
        _texture_color = 0;
    }
    if (_texture_depth != 0) {
        glDeleteTextures(1, &_texture_depth);
        _texture_depth = 0;
    }
}

void FrameBuffer::use() {
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
}

void FrameBuffer::unuse() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, _width, _height);
}
