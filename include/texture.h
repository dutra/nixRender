#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <string>

class Texture {
public:
    Texture(GLint texture_type = GL_TEXTURE_2D, std::string image = nullptr);
    ~Texture();
    void init(GLint internalFormat, GLint format, GLint type, int width = 0, int height = 0);
    void bind(GLuint pos);
    GLuint getID();

private:
    GLuint _texture;
    int _width, _height;
    GLint _texture_type;
    GLint _internalFormat, _format, _type;
    std::string _image;
};

#endif
