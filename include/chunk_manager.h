
#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include <memory>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "world_types.h"
#include "shader.h"
#include "texture.h"

typedef struct {
    uint16 type : 7;
    uint16 active: 1;
} Block;

typedef struct {
    std::unique_ptr<Block> blocks;
} Chunk;

class ChunkManager {

public:
    ChunkManager();
    ~ChunkManager();

    void init();
    void recreateBlocks();
    void render();

private:
    std::unique_ptr<Chunk> _chunks;
    GLuint _vao;
    GLuint _vbo;
    std::vector<Vertex> _vertices;
    Texture * _texture;
};

#endif
