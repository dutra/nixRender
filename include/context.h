
#ifndef CONTEXT_H
#define CONTEXT_H

#include "types.h"
#include <glm/glm.hpp>

typedef struct {
    uint32 height;
    uint32 width;
} Window;

typedef struct {
    Window window;

} Context;

typedef struct {
    Window window;
    glm::mat4 world;
    glm::mat4 projection;
} RenderContext;


#endif