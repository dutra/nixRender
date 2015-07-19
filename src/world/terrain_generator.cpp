
#include "terrain_generator.h"
#include <noise/noise.h>


void TerrainGenerator::init() {

}

void TerrainGenerator::dispose() {


}

float TerrainGenerator::getDensity(glm::vec3 position) {
    noise::module::Perlin perlin;
    perlin.SetSeed(16);
    perlin.SetOctaveCount(3);
    return perlin.GetValue(position.x / 15.0, position.y / 15.0, position.z / 15.0);

}