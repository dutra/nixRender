#include <iostream>
#include <ctime>
#include <fstream>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>

#include "render_system.h"
#include "main.h"
#include "color.h"
#include "shader.h"
#include "quad.h"
#include "frame_buffer.h"

RenderSystem::RenderSystem() {
    _settings.reset(new sf::ContextSettings());
    _settings->depthBits = 24;
    _settings->stencilBits = 8;
    _frames_counter = 0;
    _total_elapsed_secs = 0.0;
    _window.reset(new sf::Window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32), "OpenGL", sf::Style::Titlebar | sf::Style::Close, *_settings));
}

void RenderSystem::init() {
    std::cout << intro << "Initializing Rendering procedure" << std::endl;
    std::cout << reset << "==============================================" << std::endl;

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();

}

void RenderSystem::update(double delta_t) {
    check_events();


    countFPS(delta_t);
}

void RenderSystem::countFPS(double delta_t) {
    _total_elapsed_secs += delta_t;

    if (++_frames_counter == FRAMES_COUNTER) {
        std::cout << reset << "Avg. drawing time: " << blue << _total_elapsed_secs / FRAMES_COUNTER * 1000
        << reset << " ms, FPS: " << blue << 1.0 / (_total_elapsed_secs / FRAMES_COUNTER) << reset << "." << std::endl;
        _frames_counter = 0;
        _total_elapsed_secs = 0.0;
    }
}

bool RenderSystem::isWindowOpen() {
    return _window->isOpen();
}

void RenderSystem::check_events() {
    sf::Event windowEvent;
    while (_window->pollEvent(windowEvent)) {
        switch (windowEvent.type) {
            case sf::Event::Closed:
                _window->close();
            break;
            case sf::Event::KeyPressed:
                if (windowEvent.key.code == sf::Keyboard::Escape)
                    _window->close();
                break;
            default:
                break;
            }
        }
}

RenderSystem::~RenderSystem() {


}
