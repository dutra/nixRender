
#ifndef CUBE_H
#define CUBE_H

#include <memory>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "world_types.h"
#include "shader.h"
#include "texture.h"

class Cube {

public:
    Cube();
    ~Cube();

    void init();
    void recreateBlocks();
    void render();

private:
    GLuint _vao;
    GLuint _vbo;
    std::vector<Vertex> _vertices;
    Texture * _texture;
};

#endif
