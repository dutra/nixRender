#include <iostream>
#include <ctime>
#include <fstream>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "main.h"
#include "color.h"
#include "shader.h"
#include "quad.h"
#include "frame_buffer.h"

void check_events(sf::Window& window);

int main() {
    std::cout << intro << "Initializing Rendering procedure" << std::endl;
    std::cout << reset << "==============================================" << std::endl;
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    sf::Window window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32), "OpenGL", sf::Style::Titlebar | sf::Style::Close, settings);
    // fps counter
    int frames_counter{ 0 };
    double total_elapsed_secs{ 0.0 };

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    Shader first_shader("shaders/basic.vert", "shaders/basic.frag");
    first_shader.bindFragDataLocation(0, "outColor");
    first_shader.compile();
    Shader second_shader("shaders/basic_red.vert", "shaders/basic_red.frag");
    second_shader.bindFragDataLocation(0, "outColor");
    second_shader.compile();
    second_shader.setTextureUniform("texBlue", 0);
    

    Quad quad;
    
    FrameBuffer first_fbo(WINDOW_WIDTH, WINDOW_HEIGHT);
    first_fbo.init();
    FrameBuffer second_fbo(WINDOW_WIDTH, WINDOW_HEIGHT);
    second_fbo.init();

    while (window.isOpen()) {

        check_events(window);

        // Initialize counter
        std::clock_t begin = std::clock();
        first_shader.use();
        first_fbo.use();


        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        quad.draw();
        
        first_fbo.unuse();
        first_shader.unuse();
        first_fbo.bindTexture(0);
        //////////////////////////////////

        second_shader.use();
        second_fbo.use();

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        quad.draw();

        second_fbo.unuse();
        second_shader.unuse();


        // Swap buffers
        window.display();

        std::clock_t end = std::clock();
        total_elapsed_secs += double(end - begin) / CLOCKS_PER_SEC;
        if (++frames_counter == FRAMES_COUNTER) {
            std::cout << reset << "Avg. drawing time: " << blue << total_elapsed_secs / FRAMES_COUNTER * 1000 << reset << " ms, FPS: " << blue << 1.0 / (total_elapsed_secs / FRAMES_COUNTER) << reset << "." << std::endl;
            frames_counter = 0;
            total_elapsed_secs = 0;
        }
    }

}

void check_events(sf::Window& window) {
    sf::Event windowEvent;
    while (window.pollEvent(windowEvent)) {
        switch (windowEvent.type) {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::KeyPressed:
            if (windowEvent.key.code == sf::Keyboard::Escape)
                window.close();
            break;
        default:
            break;
        }
    }
}
