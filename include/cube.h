
#ifndef CUBE_H
#define CUBE_H

#include <memory>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "world_types.h"
#include "shader.h"
#include "texture.h"
#include "shader.h"

class Cube {

public:
    Cube();
    ~Cube();

    void init();
    void recreate_blocks();
    void render(std::shared_ptr<Shader> shader);

private:
    GLuint _vao;
    GLuint _vbo;
    std::vector<Vertex> _vertices;
    std::unique_ptr<Texture> _texture_color, _texture_normal;
};

#endif
