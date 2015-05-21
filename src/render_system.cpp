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
#include "gbuffer.h"
#include "chunk_manager.h"
#include "cube.h"
#include "error.h"

RenderSystem::RenderSystem() {
    _settings.reset(new sf::ContextSettings());
    _settings->depthBits = 24;
    _settings->stencilBits = 8;
    _frames_counter = 0;
    _t_total = 0.0;
    _window.reset(new sf::Window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32), "OpenGL", sf::Style::Titlebar | sf::Style::Close, *_settings));
//    _blockShader.reset(new Shader("shaders/block.vert", "shaders/block.frag"));
    _geometry_shader.reset(new Shader("shaders/deferred/geometry.vert", "shaders/deferred/geometry.frag"));
    _chunkManager.reset(new ChunkManager());
    _cube.reset(new Cube());
    _gbuffer.reset(new GBuffer(WINDOW_WIDTH, WINDOW_HEIGHT));
}

void RenderSystem::init() {
    std::cout << intro << "Initializing Rendering procedure" << std::endl;
    std::cout << reset << "==============================================" << std::endl;

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    _geometry_shader->bindFragDataLocation(0, "OutWorldPos");
    _geometry_shader->bindFragDataLocation(1, "OutDiffuse");
    _geometry_shader->bindFragDataLocation(2, "OutNormal");
    _geometry_shader->bindFragDataLocation(3, "OutTexCoord");
    _geometry_shader->compile();
    _geometry_shader->use();

    _gbuffer->init();

    // Set up view
    _view = glm::lookAt(
    glm::vec3(2.0f, 2.0f, 3.0f), // eye
    glm::vec3(0.0f, 0.0f, 0.0f), // center
    glm::vec3(0.0f, 1.0f, 0.0f) // up
    );
    GLint uniView = _geometry_shader->getUniformLocation("view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(_view));
    
    // Set up projection
    _proj = glm::perspective(FOV, ((float)WINDOW_WIDTH) / ((float)WINDOW_HEIGHT), 1.0f, 20.0f);
    GLint uniProj = _geometry_shader->getUniformLocation("proj");
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(_proj));
    
    // Set up model
    _world = glm::mat4();
    GLint uniModel = _geometry_shader->getUniformLocation("world");
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(_world));
    
    _chunkManager->init();
    _cube->init();
    
    _geometry_shader->unuse();
    
    glCheckError();
}

void RenderSystem::update(double delta_t) {
    // use shader
    _geometry_shader->use();
    // use geometry buffer
    _gbuffer->use();
    
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnable(GL_DEPTH_TEST);

    // render all components
    //_chunkManager->render();
    _cube->render();

    //_blockShader->unuse();
    _gbuffer->unuse();
    _geometry_shader->unuse();

    _window->display();

    check_events();
    countFPS(delta_t);
}

void RenderSystem::countFPS(double delta_t) {
    _t_total += delta_t;

    if (++_frames_counter == FRAMES_COUNTER) {
        std::cout << reset << "Avg. drawing time: " << blue << _t_total / FRAMES_COUNTER
        << reset << " ms, FPS: " << blue << 1.0 / (_t_total / FRAMES_COUNTER)*1000 << reset << "." << std::endl;
        _frames_counter = 0;
        _t_total = 0.0;
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
