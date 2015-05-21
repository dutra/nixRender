

#include "cube.h"
#include <memory>
#include <glm/glm.hpp>
#include "main.h"
#include "texture.h"

Cube::Cube() {
    
}

void Cube::init() {
    recreateBlocks();
}

void Cube::recreateBlocks() {
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    // front
    _vertices.push_back(Vertex{ 0.0f, 0.0f, 1.0f }); //A
    _vertices.push_back(Vertex{ 1.0f, 0.0f, 1.0f }); //B
    _vertices.push_back(Vertex{ 1.0f, 1.0f, 1.0f }); //C
    _vertices.push_back(Vertex{ 1.0f, 1.0f, 1.0f }); //D
    _vertices.push_back(Vertex{ 0.0f, 1.0f, 1.0f }); //E
    _vertices.push_back(Vertex{ 0.0f, 0.0f, 1.0f }); //F

    // top
    _vertices.push_back(Vertex{ 0.0f, 1.0f, 1.0f }); //G
    _vertices.push_back(Vertex{ 1.0f, 1.0f, 1.0f }); //H
    _vertices.push_back(Vertex{ 0.0f, 1.0f, 0.0f }); //I
    _vertices.push_back(Vertex{ 1.0f, 1.0f, 1.0f }); //J
    _vertices.push_back(Vertex{ 1.0f, 1.0f, 0.0f }); //L
    _vertices.push_back(Vertex{ 0.0f, 1.0f, 0.0f }); //M

    // back
    _vertices.push_back(Vertex{ 1.0f, 1.0f, 0.0f }); //N
    _vertices.push_back(Vertex{ 1.0f, 0.0f, 0.0f }); //O
    _vertices.push_back(Vertex{ 0.0f, 0.0f, 0.0f }); //P
    _vertices.push_back(Vertex{ 0.0f, 0.0f, 0.0f }); //Q
    _vertices.push_back(Vertex{ 0.0f, 1.0f, 0.0f }); //R
    _vertices.push_back(Vertex{ 1.0f, 1.0f, 0.0f }); //S

    // bottom
    _vertices.push_back(Vertex{ 0.0f, 0.0f, 1.0f });
    _vertices.push_back(Vertex{ 0.0f, 0.0f, 0.0f });
    _vertices.push_back(Vertex{ 1.0f, 0.0f, 1.0f });
    _vertices.push_back(Vertex{ 1.0f, 0.0f, 1.0f });
    _vertices.push_back(Vertex{ 0.0f, 0.0f, 0.0f });
    _vertices.push_back(Vertex{ 1.0f, 0.0f, 0.0f });

    // left
    _vertices.push_back(Vertex{ 0.0f, 1.0f, 1.0f });
    _vertices.push_back(Vertex{ 0.0f, 1.0f, 0.0f });
    _vertices.push_back(Vertex{ 0.0f, 0.0f, 0.0f });
    _vertices.push_back(Vertex{ 0.0f, 0.0f, 0.0f });
    _vertices.push_back(Vertex{ 0.0f, 0.0f, 1.0f });
    _vertices.push_back(Vertex{ 0.0f, 1.0f, 1.0f });

    // right
    _vertices.push_back(Vertex{ 1.0f, 1.0f, 1.0f });
    _vertices.push_back(Vertex{ 1.0f, 0.0f, 1.0f });
    _vertices.push_back(Vertex{ 1.0f, 1.0f, 0.0f });
    _vertices.push_back(Vertex{ 1.0f, 1.0f, 0.0f });
    _vertices.push_back(Vertex{ 1.0f, 0.0f, 1.0f });
    _vertices.push_back(Vertex{ 1.0f, 0.0f, 0.0f });
    
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*_vertices.size(), &_vertices.front(), GL_STATIC_DRAW);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    //GLint posAttrib = glGetAttribLocation(_shader->getProgram(), "position");
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, // index
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized
        0, //4 * sizeof(GLfloat), // + sizeof(GLubyte), // stride (byte offset between consecutive generic vertex attributes)
        0); // pointer (offset of the first component of the first generic vertex attribute)


    /*    GLint materialAttrib = glGetAttribLocation(_shader->getProgram(), "material");
    glEnableVertexAttribArray(materialAttrib);
    glVertexAttribPointer(materialAttrib, 3, GL_UNSIGNED_BYTE, GL_FALSE, 4 * sizeof(GLfloat) + sizeof(GLubyte), (void*)(3 * sizeof(GLfloat)));
    */
    /*
    int layer = 0;

    _texture->init(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);

    GLubyte *texData;

    texData = new GLubyte[15*15*15 * 4];
    for(int i = 0; i<15*15*15*4; i++) {
    texData[i] = 0;
    }
    int x = 2, y = 0, z = 0;
    texData[x/4 + y/4*15 + z/4*15*15] = 1;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_1D_ARRAY, _texture->getID());
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexSubImage2D(GL_TEXTURE_1D_ARRAY, 0, 0, 0, 15*15*15, 0, GL_RGBA, GL_UNSIGNED_INT, texData);
    delete[] texData;*/


}

void Cube::render() {
    glBindVertexArray(_vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glDrawArrays(GL_TRIANGLES, 0, 6 * 6);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);

}

Cube::~Cube() {
    glDeleteBuffers(1, &_vbo);
    glDeleteVertexArrays(1, &_vao);
    delete _texture;
}
