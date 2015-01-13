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
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    _vertices.push_back(Vertex{ -1.0f, 1.0f, 0.0f  });
    _vertices.push_back(Vertex{ 1.0f, 1.0f, 0.0f   });
    _vertices.push_back(Vertex{ 1.0f, -1.0f, 0.0f  });
    _vertices.push_back(Vertex{ 1.0f, -1.0f, 0.0f  });
    _vertices.push_back(Vertex{ -1.0f, -1.0f, 0.0f });
    _vertices.push_back(Vertex{ -1.0f, 1.0f, 0.0f  });

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*_vertices.size(), &_vertices.front(), GL_STATIC_DRAW);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


}

Quad::~Quad() {
    glDeleteBuffers(1, &_vbo);
    glDeleteVertexArrays(1, &_vao);

}
