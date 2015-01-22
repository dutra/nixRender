#include <iostream>
#include <ctime>
#include <fstream>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>

#include "main.h"
#include "color.h"
#include "render_system.h"
#include <experimental/optional>
int main() {
    std::clock_t last_time = std::clock();
    RenderSystem rs;
    rs.init();
//    Quad quad;


    while (rs.isWindowOpen()) {
        std::clock_t current = std::clock();
        double elapsed_secs = double(current - last_time) / CLOCKS_PER_SEC;

        rs.update(elapsed_secs);

        // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        //
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //
        // quad.draw();

        // Swap buffers
        //window.display();

        last_time = std::clock();

    }


}
