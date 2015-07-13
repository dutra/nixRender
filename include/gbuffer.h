#ifndef GBUFFER_H
#define GBUFFER_H

#include <GL/glew.h>
#include <memory>
#include "shader.h"

enum GBUFFER_TEXTURE_TYPE {
    GBUFFER_TEXTURE_TYPE_POSITION,
    GBUFFER_TEXTURE_TYPE_NORMAL,
    GBUFFER_TEXTURE_TYPE_DIFFUSE,
    GBUFFER_TEXTURE_TYPE_TEXCOORD,
    GBUFFER_NUM_TEXTURES
};


class GBuffer {
public:
    GBuffer(int width, int height);
    ~GBuffer();
    void init();
    void dispose();
    void use();
    void unuse();
    void read(std::shared_ptr<Shader> shader);
    void unread();

private:
    GLuint _fbo;
    GLuint _textures[GBUFFER_NUM_TEXTURES];
    GLuint _depth_texture;
    int _width, _height;
};

#endif