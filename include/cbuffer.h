#ifndef CBUFFER_H
#define CBUFFER_H

#include <GL/glew.h>
#include <memory>
#include "shader.h"
#include "types.h"

enum CBUFFER_TEXTURE_TYPE {
    CBUFFER_TEXTURE_TYPE_AMBIENT,
    CBUFFER_TEXTURE_TYPE_DIFFUSE,
    CBUFFER_TEXTURE_TYPE_SPECULAR,
    CBUFFER_NUM_TEXTURES
};


class CBuffer {
public:
    CBuffer(int width, int height);
    ~CBuffer();
    void init();
    void dispose();
    void use();
    void unuse();
    void read(std::shared_ptr<Shader> shader, size_t pos);
    void unread();

private:
    GLuint _fbo;
    GLuint _textures[CBUFFER_NUM_TEXTURES];
    uint32 _width, _height;
    size_t m_pos;
};

#endif