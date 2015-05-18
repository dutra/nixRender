#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

class Texture {
public:
    Texture(int window_width, int window_height, GLint texture_type = GL_TEXTURE_2D);
    ~Texture();
    void init(GLint internalFormat, GLint format, GLint type);
    void bind(GLuint pos);
    GLuint getID();

private:
    GLuint _texture;
    int _width, _height;
    GLint _texture_type;
    GLint _internalFormat, _format, _type;
};

#endif
