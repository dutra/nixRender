#include "texture.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "color.h"

Texture::Texture(int window_width, int window_height, GLint texture_type)
: _width(window_width), _height(window_height), _texture_type(texture_type) {

}

void Texture::init(GLint internalFormat, GLint format, GLint type) {
    _internalFormat = internalFormat;
    _format = format;
    _type = type;

    if(_texture_type == GL_TEXTURE_2D) {
        // Create texture to hold color buffer
        glGenTextures(1, &_texture);
        glBindTexture(GL_TEXTURE_2D, _texture);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _width, _height, 0, format, type, nullptr);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Unbind used resources
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    if (_texture_type == GL_TEXTURE_1D_ARRAY) {
        glGenTextures(1, &_texture);
        glBindTexture(GL_TEXTURE_1D_ARRAY, _texture);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _width, _height, 0, format, type, nullptr);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    }
}

Texture::~Texture() {
    if (_texture != 0) {
        glDeleteTextures(1, &_texture);
        _texture = 0;
    }
}


GLuint Texture::getID() {
    if (glIsTexture(_texture) == GL_FALSE) {
        std::cout << error << "Error: Texture doesn't exist." << std::endl;
        assert(false);
    }
    return _texture;
}

void Texture::bind(GLuint pos) {
    if (glIsTexture(_texture) == GL_FALSE) {
        std::cout << error << "Error: Trying to bind texture that doesn't exist." << std::endl;
        assert(false);
    }

    glActiveTexture(GL_TEXTURE0 + pos);
    if (_texture_type == GL_TEXTURE_2D)
        glBindTexture(GL_TEXTURE_2D, _texture);
    if (_texture_type == GL_TEXTURE_1D_ARRAY)
        glBindTexture(GL_TEXTURE_1D_ARRAY, _texture);
}
