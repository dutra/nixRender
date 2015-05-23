#ifndef QUAD_H
#define QUAD_H

#include "shader.h"

#include <GL/glew.h>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "world_types.h"

class Quad {

public:
    void draw();
    Quad();
    ~Quad();
    void init();

private:
    GLuint _vao;
    GLuint _vbo;
    std::vector<VertexQuad> _vertices;

};

#endif // QUAD_H
