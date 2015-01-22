#ifndef WORLD_TYPES_H
#define WORLD_TYPES_H

enum class Material {
    RADIOACTIVE
};

enum class Primitive {
    CUBE
};

typedef struct {
    glm::vec3 position;
    glm::vec3 rotation;
} StaticComponent;

typedef struct {
    Material material;
    Primitive primitive;
} DrawComponent;

#endif /* WORLD_TYPES_H */
