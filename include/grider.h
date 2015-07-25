

#ifndef GRIDER_H
#define GRIDER_H

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