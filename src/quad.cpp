
#include "quad.h"
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "color.h"

void Quad::draw() {

    glBindVertexArray(_vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    // Draw quad
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);

}

Quad::Quad() {
}

Quad::~Quad() {
    glDeleteBuffers(1, &_vbo);
    glDeleteVertexArrays(1, &_vao);

}


void Quad::init() {
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    _vertices.push_back(VertexQuad{ -1.0f, 1.0f, 0.0f, 0.0f, 1.0f });
    _vertices.push_back(VertexQuad{ 1.0f, 1.0f, 0.0f, 1.0f, 1.0f });
    _vertices.push_back(VertexQuad{ 1.0f, -1.0f, 0.0f, 1.0f, 0.0f });
    _vertices.push_back(VertexQuad{ 1.0f, -1.0f, 0.0f, 1.0f, 0.0f });
    _vertices.push_back(VertexQuad{ -1.0f, -1.0f, 0.0f, 0.0f, 0.0f });
    _vertices.push_back(VertexQuad{ -1.0f, 1.0f, 0.0f, 0.0f, 1.0f });

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*_vertices.size(), &_vertices.front(), GL_STATIC_DRAW);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, // index
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized
        5 * sizeof(GLfloat), //4 * sizeof(GLfloat), // + sizeof(GLubyte), // stride (byte offset between consecutive generic vertex attributes)
        (void*)(0 * sizeof(GLfloat))); // pointer (offset of the first component of the first generic vertex attribute)

    // UV
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, // index
        2, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized
        5 * sizeof(GLfloat), //4 * sizeof(GLfloat), // + sizeof(GLubyte), // stride (byte offset between consecutive generic vertex attributes)
        (void*)(3 * sizeof(GLfloat))); // pointer (offset of the first component of the first generic vertex attribute)


}