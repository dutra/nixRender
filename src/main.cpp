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
    int frames_counter(0);
    double total_elapsed_secs(0.0);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    Quad quad;

    //////////////////////////////////////////////////////////////////////////
    // Transmittance
    Shader transmittanceShader("shaders/atmo/std.vert", "shaders/atmo/transmittance.frag", "shaders/atmo/common.glsl");
    transmittanceShader.bindFragDataLocation(0, "outColor");
    transmittanceShader.compile();
    FrameBuffer transmittanceFbo(TRANSMITTANCE_W, TRANSMITTANCE_H);
    transmittanceFbo.init();
    transmittanceShader.use();
    transmittanceFbo.use();
    quad.draw();
    
    transmittanceFbo.unuse(WINDOW_WIDTH, WINDOW_HEIGHT);
    transmittanceShader.unuse();
    

    //////////////////////////////////////////////////////////////////////////
    // Irradiance1
    Shader irradiance1Shader("shaders/atmo/std.vert", "shaders/atmo/irradiance1.frag", "shaders/atmo/common.glsl");
    irradiance1Shader.bindFragDataLocation(0, "outColor");
    irradiance1Shader.compile();
    
    FrameBuffer irradianceFbo(SKY_W, SKY_H);
    irradianceFbo.init();
    irradiance1Shader.setTextureUniform("transmittanceSampler", 2);

    irradiance1Shader.use();
    irradianceFbo.use();
       
    transmittanceFbo.bindTexture(2);
    quad.draw();

    irradianceFbo.unuse(WINDOW_WIDTH, WINDOW_HEIGHT);
    irradiance1Shader.unuse();



    while (window.isOpen()) {

        check_events(window);

        // Initialize counter
        std::clock_t begin = std::clock();


        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        

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