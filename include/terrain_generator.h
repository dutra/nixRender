
#ifndef TERRAIN_GENERATOR_H
#define TERRAIN_GENERATOR_H

#include <glm/glm.hpp>

class TerrainGenerator {
public:
    void init();
    void dispose();
    float getDensity(glm::vec3 position);
};

#endif