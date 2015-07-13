
#ifndef MARCHING_CUBE_MESHER_H
#define MARCHING_CUBE_MESHER_H
#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include "types.h"

#define GRID_SIZE_X 32
#define GRID_SIZE_Y 32
#define GRID_SIZE_Z 32
#define GRID_SIZE GRID_SIZE_X*GRID_SIZE_Y*GRID_SIZE_Z

typedef struct {
    float x, y, z; // position
    float nx, ny, nz; // normal
} VertexNormal;

typedef struct {
    VertexNormal p[3];
} Triangle;

typedef struct {
    glm::vec3 p[8];
    float val[8]; // density values at each 
} Gridcell;

class MarchingCubeMesher {

public:
    void init();
    void render();
    ~MarchingCubeMesher();

private:
    int polygonise(Gridcell grid, float isolevel, Triangle *triangles);
    VertexNormal vertexInterpolate(float isolevel, glm::vec3 p1, glm::vec3 p2, float valp1, float valp2);
    void generateGrid();
    void generateTriagles();

    GLuint _vao;
    GLuint _vbo;
    uint32 m_totalTriangles;

    Gridcell ***_grid;
    Triangle *_triangles;
	static const unsigned int m_edgeTable[256];
    static const unsigned int m_triTable[256][16];
};


#endif