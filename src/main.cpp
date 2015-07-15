#include <iostream>
#include <chrono>
#include <fstream>
#include <cmath>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>

#include "main.h"
#include "color.h"
#include "render_system.h"
#include "world_manager.h"
#include "world_types.h"

int main() {


    auto t_last = std::chrono::high_resolution_clock::now();
    RenderSystem rs;
    rs.init();

    WorldManager wm;
    wm.addComponent(55, Component::DRAW_COMPONENT);

    std::cout << "GL_MAX_TEXTURE_SIZE" << GL_MAX_TEXTURE_SIZE << std::endl;
    std::cout << "GL_MAX_ARRAY_TEXTURE_LAYERS" << GL_MAX_ARRAY_TEXTURE_LAYERS << std::endl;

    while (rs.isWindowOpen()) {
        auto t_current = std::chrono::high_resolution_clock::now();
        double delta_t = std::chrono::duration<double, std::milli>(t_current-t_last).count();

        rs.update(delta_t);

        t_last = t_current;

    }
    

}
