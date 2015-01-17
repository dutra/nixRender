#include "frame_buffer.h"

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "color.h"
FrameBuffer::FrameBuffer(int window_width, int window_height, int layers)
                        : _width(window_width), _height(window_height), _layers(layers) {

}

void FrameBuffer::init() {
    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

    // Create texture to hold color buffer
    glGenTextures(1, &_texture_color);
    glBindTexture(GL_TEXTURE_2D, _texture_color);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _width, _height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Attach the texture to the color buffer in our frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture_color, 0);

    // Set the output location for pixels
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    // Check that status of our generated frame buffer
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);   
    if (status != GL_FRAMEBUFFER_COMPLETE) { 
        std::cout << error << "ERROR: Couldn't create frame buffer" << std::endl; 
        assert(false);
    }

    // Unbind used resources
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


}

void FrameBuffer::init3d() {

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &_texture_color_3d);
    glBindTexture(GL_TEXTURE_3D, _texture_color_3d);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA16F, _width, _height, _layers, 0, GL_RGBA, GL_FLOAT, NULL);

    
    // Unbind used resources
    glBindTexture(GL_TEXTURE_2D, 0);
    
}

void FrameBuffer::drawLayer(int layer) {
    float *texData = new float[_width * _height * _layers * 4];
    glActiveTexture(GL_TEXTURE0);
    bindTexture(0);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, texData);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, _texture_color_3d);
    glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, layer, _width, _height, 1, GL_RGBA, GL_FLOAT, texData);
    delete[] texData;
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
    glViewport(0, 0, _width, _height);
}

void FrameBuffer::unuse(int window_width, int window_height) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, window_width, window_height);
}

GLuint FrameBuffer::getTextureID() {
    return _texture_color;

}

void FrameBuffer::bindTexture(GLuint pos) {
    glActiveTexture(GL_TEXTURE0 + pos);
    glBindTexture(GL_TEXTURE_2D, _texture_color);
}
