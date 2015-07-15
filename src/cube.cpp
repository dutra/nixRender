

#include "cube.h"
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "main.h"
#include "texture.h"

Cube::Cube(std::string color_map, std::string normal_map) {
    _texture_color.reset(new Texture(GL_TEXTURE_2D, color_map));
    _texture_normal.reset(new Texture(GL_TEXTURE_2D, normal_map));
}

void Cube::init() {
    recreate_blocks();
    _texture_color->init(GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
    _texture_normal->init(GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
}


void Cube::recreate_blocks() {
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    /*
      ________
     / |     /|
    /__|____/ |
    |  |____|_| 
    | /     | /
    |/______|/
    */
    // front
    _vertices.push_back(VertexNormalUV{ 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f }); //A
    _vertices.push_back(VertexNormalUV{ 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f }); //B
    _vertices.push_back(VertexNormalUV{ 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f }); //C
    _vertices.push_back(VertexNormalUV{ 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f }); //D
    _vertices.push_back(VertexNormalUV{ 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f }); //E
    _vertices.push_back(VertexNormalUV{ 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f }); //F

    // top
    _vertices.push_back(VertexNormalUV{ 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f }); //G
    _vertices.push_back(VertexNormalUV{ 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f }); //H
    _vertices.push_back(VertexNormalUV{ 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f }); //I
    _vertices.push_back(VertexNormalUV{ 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f }); //J
    _vertices.push_back(VertexNormalUV{ 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f }); //L
    _vertices.push_back(VertexNormalUV{ 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f }); //M

    // back
    _vertices.push_back(VertexNormalUV{ 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f }); //N
    _vertices.push_back(VertexNormalUV{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f }); //O
    _vertices.push_back(VertexNormalUV{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f }); //P
    _vertices.push_back(VertexNormalUV{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f }); //Q
    _vertices.push_back(VertexNormalUV{ 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f }); //R
    _vertices.push_back(VertexNormalUV{ 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f }); //S

    // bottom
    _vertices.push_back(VertexNormalUV{ 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f });
    _vertices.push_back(VertexNormalUV{ 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f });
    _vertices.push_back(VertexNormalUV{ 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f });
    _vertices.push_back(VertexNormalUV{ 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f });
    _vertices.push_back(VertexNormalUV{ 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f });
    _vertices.push_back(VertexNormalUV{ 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f });

    // left
    _vertices.push_back(VertexNormalUV{ 0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f });
    _vertices.push_back(VertexNormalUV{ 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f });
    _vertices.push_back(VertexNormalUV{ 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f });
    _vertices.push_back(VertexNormalUV{ 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f });
    _vertices.push_back(VertexNormalUV{ 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f });
    _vertices.push_back(VertexNormalUV{ 0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f });

    // right
    _vertices.push_back(VertexNormalUV{ 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f });
    _vertices.push_back(VertexNormalUV{ 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f });
    _vertices.push_back(VertexNormalUV{ 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f });
    _vertices.push_back(VertexNormalUV{ 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f });
    _vertices.push_back(VertexNormalUV{ 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f });
    _vertices.push_back(VertexNormalUV{ 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f });
    
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexNormalUV)*_vertices.size(), &_vertices.front(), GL_STATIC_DRAW);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    //GLint posAttrib = glGetAttribLocation(_shader->getProgram(), "position");
    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, // index
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized
        8 * sizeof(GLfloat), //4 * sizeof(GLfloat), // + sizeof(GLubyte), // stride (byte offset between consecutive generic vertex attributes)
        0); // pointer (offset of the first component of the first generic vertex attribute)

    // normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, // index
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized
        8 * sizeof(GLfloat), //4 * sizeof(GLfloat), // + sizeof(GLubyte), // stride (byte offset between consecutive generic vertex attributes)
        (void*)(3*sizeof(GLfloat))); // pointer (offset of the first component of the first generic vertex attribute)

    // UV
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, // index
        2, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized
        8 * sizeof(GLfloat), //4 * sizeof(GLfloat), // + sizeof(GLubyte), // stride (byte offset between consecutive generic vertex attributes)
        (void*)(6 * sizeof(GLfloat))); // pointer (offset of the first component of the first generic vertex attribute)



}

void Cube::translate(glm::vec3 pos) {
	_model = glm::translate(_model, pos);
}

void Cube::scale(glm::vec3 factor) {
	_model = glm::scale(_model, factor);
}

void Cube::render(std::shared_ptr<Shader> shader) {
	// translate and scale model first
	GLint uniModel = shader->getUniformLocation("world");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(_model));

	// bind Vertex Array
	glBindVertexArray(_vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    shader->setTextureUniform("colorMap", 0);
    _texture_color->bind(0);
    shader->setTextureUniform("normalMap", 1);
    _texture_color->bind(1);

	// draw
    glDrawArrays(GL_TRIANGLES, 0, 6 * 6);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);

}

Cube::~Cube() {
    glDeleteBuffers(1, &_vbo);
    glDeleteVertexArrays(1, &_vao);
}
