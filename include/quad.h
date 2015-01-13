#ifndef QUAD_H
#define QUAD_H

#include <GL/glew.h>
#include <string>
#include <vector>
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

typedef struct {
    float x, y, z;
} Vertex;

class Quad {

public:
    void draw();
    Quad();
    ~Quad();

private:
    GLuint _vao;
    GLuint _vbo;
    std::vector<Vertex> _vertices;

};

#endif // QUAD_H
