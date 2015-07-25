
#ifndef TERRAIN_GENERATOR_H
#define TERRAIN_GENERATOR_H

#include <glm/glm.hpp>
#include <memory>
#include "grider.h"

class TerrainGenerator {
public:
    void init();
    void dispose();
    float getDensity(glm::vec3 position);
private:
    std::unique_ptr<Grider> m_grider;

};

#endif