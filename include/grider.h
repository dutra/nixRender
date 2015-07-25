

#ifndef GRIDER_H
#define GRIDER_H

#define GRID_NEAR_SIZE_X 128
#define GRID_NEAR_SIZE_Y 36
#define GRID_NEAR_SIZE_Z 128

#define GRID_MEDIUM_SIZE_X 128
#define GRID_MEDIUM_SIZE_Y 36
#define GRID_MEDIUM_SIZE_Z 128

#define GRID_MEDIUM_FACTOR_X 4
#define GRID_MEDIUM_FACTOR_Y 4
#define GRID_MEDIUM_FACTOR_Z 4

#define GRID_TOTAL_SIZE_X GRID_NEAR_SIZE_X + GRID_MEDIUM_SIZE_X
#define GRID_TOTAL_SIZE_Y GRID_NEAR_SIZE_Y + GRID_MEDIUM_SIZE_Y
#define GRID_TOTAL_SIZE_Z GRID_NEAR_SIZE_Z + GRID_MEDIUM_SIZE_Z

#include <functional>

#include "world_types.h"

class Grider {
public:
    void init(std::function<float(glm::vec3)> density);
    Gridcell*** getGrid();
    ~Grider();

private:
    void fillGrid();
    std::function<float(glm::vec3)> m_density;
    Gridcell ***m_grid;
    glm::vec3 m_position, m_center_position;
};

#endif