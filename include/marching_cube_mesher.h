
#ifndef MARCHING_CUBE_MESHER_H
#define MARCHING_CUBE_MESHER_H
#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include "types.h"
#include "world_types.h"

#define GRID_SIZE_X 32
#define GRID_SIZE_Y 32
#define GRID_SIZE_Z 32
#define GRID_SIZE GRID_SIZE_X*GRID_SIZE_Y*GRID_SIZE_Z

class MarchingCubeMesher {

public:
    void init();
    void render();
    ~MarchingCubeMesher();

private:
    glm::vec3 gradient(glm::vec3 position);
    int polygonise(Gridcell grid, float isolevel, Triangle *triangles);
    glm::vec3 vertexInterpolate(float isolevel, glm::vec3 p1, glm::vec3 p2, float valp1, float valp2);
    void generateGrid();
    void generateTriagles();
    void computeNormal(VertexNormal& p1, VertexNormal& p2, VertexNormal& p3);

    GLuint _vao;
    GLuint _vbo;
    uint32 m_totalTriangles;

    Gridcell ***_grid;
    Triangle *_triangles;
	static const unsigned int m_edgeTable[256];
    static const unsigned int m_triTable[256][16];
};


#endif