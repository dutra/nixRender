#ifndef WORLD_TYPES_H
#define WORLD_TYPES_H

#include "types.h"

enum class Material {
    WOOD, SAND
};

typedef struct {
    glm::vec3 position;
    glm::vec3 rotation;
} StaticComponent;

typedef struct {
    Material material;
} DrawComponent;

enum class Component {
    DRAW_COMPONENT = 0x0,
    STATIC_COMPONENT = 0x01
};

typedef struct {
    uuid id;
} Entity;

typedef struct {
    float x, y, z; // position
    float nx, ny, nz; // normal
    float fU, fV; // texture
} Vertex;

typedef struct {
    float x, y, z; // position
    float fU, fV; // texture
} VertexQuad;

#endif /* WORLD_TYPES_H */
