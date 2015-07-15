
#include "chunk_manager.h"
#include <memory>
#include <glm/glm.hpp>
#include "main.h"
#include "texture.h"

ChunkManager::ChunkManager() {
    //_shader = shader;
    _chunks.reset(new Chunk);

    _chunks->blocks.reset(new Block [CHUNK_SIZE_X*CHUNK_SIZE_Y*CHUNK_SIZE_Z]);
    //_texture = new Texture(15*15*15, 1, GL_TEXTURE_1D_ARRAY);
}

void ChunkManager::init() {
    recreateBlocks();
}

void ChunkManager::recreateBlocks() {
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    // vertices
    for(int k = 0; k < CHUNK_SIZE_Z; k++) {
        for(int j = 0; j < CHUNK_SIZE_Y; j++) {
            for(int i = 0; i < CHUNK_SIZE_X; i++) {
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
                _vertices.push_back(VertexNormalUV{ 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f }); //T
                _vertices.push_back(VertexNormalUV{ 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f }); //U
                _vertices.push_back(VertexNormalUV{ 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f }); //V
                _vertices.push_back(VertexNormalUV{ 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f }); //X
                _vertices.push_back(VertexNormalUV{ 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f }); //W
                _vertices.push_back(VertexNormalUV{ 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f }); //Z

                // left
                _vertices.push_back(VertexNormalUV{ 0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f }); //a
                _vertices.push_back(VertexNormalUV{ 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f }); //b
                _vertices.push_back(VertexNormalUV{ 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f }); //c
                _vertices.push_back(VertexNormalUV{ 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f }); //d
                _vertices.push_back(VertexNormalUV{ 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f }); //e
                _vertices.push_back(VertexNormalUV{ 0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f }); //f

                // right
                _vertices.push_back(VertexNormalUV{ 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f }); //g
                _vertices.push_back(VertexNormalUV{ 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f }); //h
                _vertices.push_back(VertexNormalUV{ 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f }); //i
                _vertices.push_back(VertexNormalUV{ 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f }); //j
                _vertices.push_back(VertexNormalUV{ 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f }); //l
                _vertices.push_back(VertexNormalUV{ 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f }); //m
            }
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexNormalUV)*_vertices.size(), &_vertices.front(), GL_STATIC_DRAW);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

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
        (void*)(3 * sizeof(GLfloat))); // pointer (offset of the first component of the first generic vertex attribute)

    // UV
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, // index
        2, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized
        8 * sizeof(GLfloat), //4 * sizeof(GLfloat), // + sizeof(GLubyte), // stride (byte offset between consecutive generic vertex attributes)
        (void*)(6 * sizeof(GLfloat))); // pointer (offset of the first component of the first generic vertex attribute)


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

void ChunkManager::render() {
    glBindVertexArray(_vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glDrawArrays(GL_TRIANGLES, 0, 6*6);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);

}

ChunkManager::~ChunkManager() {
    glDeleteBuffers(1, &_vbo);
    glDeleteVertexArrays(1, &_vao);
//    delete _texture;
}
