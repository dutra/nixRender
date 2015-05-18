

#include "chunk_manager.h"
#include <memory>
#include <glm/glm.hpp>
#include "main.h"
#include "texture.h"

ChunkManager::ChunkManager(std::shared_ptr<Shader> shader) {
    //_shader = shader;
    _chunks.reset(new Chunk);

    _chunks->blocks.reset(new Block [CHUNK_SIZE_X*CHUNK_SIZE_Y*CHUNK_SIZE_Z]);
    _texture = new Texture(15*15*15, 1, GL_TEXTURE_1D_ARRAY);
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
                _vertices.push_back(Vertex{ 0.0f, 0.0f, 1.0f });
                _vertices.push_back(Vertex{ 1.0f, 0.0f, 1.0f });
                _vertices.push_back(Vertex{ 1.0f, 1.0f, 1.0f });
                _vertices.push_back(Vertex{ 1.0f, 1.0f, 1.0f });
                _vertices.push_back(Vertex{ 0.0f, 1.0f, 1.0f });
                _vertices.push_back(Vertex{ 0.0f, 0.0f, 1.0f });

                // top
                _vertices.push_back(Vertex{ 0.0f, 1.0f, 1.0f });
                _vertices.push_back(Vertex{ 1.0f, 1.0f, 1.0f });
                _vertices.push_back(Vertex{ 0.0f, 0.0f, 0.0f });
                _vertices.push_back(Vertex{ 1.0f, 1.0f, 1.0f });
                _vertices.push_back(Vertex{ 1.0f, 0.0f, 0.0f });
                _vertices.push_back(Vertex{ 0.0f, 0.0f, 0.0f });

                // back
                _vertices.push_back(Vertex{ 1.0f, 1.0f, 0.0f });
                _vertices.push_back(Vertex{ 1.0f, 0.0f, 0.0f });
                _vertices.push_back(Vertex{ 0.0f, 0.0f, 0.0f });
                _vertices.push_back(Vertex{ 0.0f, 0.0f, 0.0f });
                _vertices.push_back(Vertex{ 0.0f, 1.0f, 0.0f });
                _vertices.push_back(Vertex{ 1.0f, 1.0f, 0.0f });

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
                _vertices.push_back(Vertex{  1.0f, 1.0f, 1.0f });
                _vertices.push_back(Vertex{ 1.0f, 0.0f, 1.0f });
                _vertices.push_back(Vertex{ 1.0f, 1.0f, 0.0f });
                _vertices.push_back(Vertex{ 1.0f, 1.0f, 0.0f });
                _vertices.push_back(Vertex{ 1.0f, 0.0f, 1.0f });
                _vertices.push_back(Vertex{ 1.0f, 0.0f, 0.0f });


            }
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*_vertices.size(), &_vertices.front(), GL_STATIC_DRAW);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    //GLint posAttrib = glGetAttribLocation(_shader->getProgram(), "position");
    //glEnableVertexAttribArray(posAttrib);
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
    delete[] texData;


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
    delete _texture;
}
