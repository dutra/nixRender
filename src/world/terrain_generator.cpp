
#include "terrain_generator.h"
#include <noise/noise.h>


void TerrainGenerator::init() {

}

void TerrainGenerator::dispose() {


}

float TerrainGenerator::getDensity(glm::vec3 position) {
    noise::module::Perlin perlin;
    perlin.SetSeed(19);
    perlin.SetOctaveCount(3);

    float density = -position.y;
    density += perlin.GetValue(position.x / 40.0, position.y / 40.0, position.z / 40.0)*50;
//    density += perlin.GetValue(position.x, position.y, position.z)*1;
    return density;
//    return perlin.GetValue(position.x / 15.0, position.y / 15.0, position.z / 15.0);

}