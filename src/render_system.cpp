#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include <sstream>
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
#include "marching_cube_mesher.h"


RenderSystem::RenderSystem() {
    _settings.reset(new sf::ContextSettings());
    _settings->depthBits = 24;
    _settings->stencilBits = 8;
	_settings->antialiasingLevel = 8;

    _frames_counter = 0;
    _t_total = 0.0;
    _window.reset(new sf::Window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32), "OpenGL", sf::Style::Titlebar | sf::Style::Close, *_settings));
    _clear_shader.reset(new Shader("shaders/deferred/clear.vert", "shaders/deferred/clear.frag"));
    _lighting_shader.reset(new Shader("shaders/deferred/lighting.vert", "shaders/terrain/lighting.frag"));
    _pass_shader.reset(new Shader("shaders/pass.vert", "shaders/pass.frag"));
    _chunkManager.reset(new ChunkManager());
	_quad.reset(new Quad);
	m_GBuffer.reset(new GBuffer(WINDOW_WIDTH, WINDOW_HEIGHT));
    m_CBuffer.reset(new CBuffer(WINDOW_WIDTH, WINDOW_HEIGHT));
    _camera.reset(new Camera);
    m_terrainGRenderStage.reset(new TerrainRenderStage);
}

void RenderSystem::init() {
    std::cout << intro << "Initializing Rendering procedure" << std::endl;
    std::cout << reset << "==============================================" << std::endl;

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    // camera
    _camera->init();

    // lighting shader
    _lighting_shader->bindFragDataLocation(0, "OutColor");
    _lighting_shader->compile();
    //_lighting_shader->use();


	// pass shader
	_pass_shader->bindFragDataLocation(0, "TexCoord0");
	_pass_shader->compile();

    // clear shader
    _clear_shader->bindFragDataLocation(0, "OutWorldPos");
    _clear_shader->bindFragDataLocation(1, "OutNormal");
    _clear_shader->compile();

    m_GBuffer->init();
    m_CBuffer->init();


    // Set up projection
    m_renderContext.projection = glm::perspective(FOV, ((float)WINDOW_WIDTH) / ((float)WINDOW_HEIGHT), 0.1f, 1000.0f);
    // Set up model
    m_renderContext.world = glm::mat4();
    m_renderContext.window.height = WINDOW_HEIGHT;
    m_renderContext.window.width = WINDOW_WIDTH;


    //_chunkManager->init();
    _quad->init();


    m_terrainGRenderStage->init(m_GBuffer, m_CBuffer, m_renderContext, _camera);
    m_terrainGRenderStage->generateTerrain();

    glCheckError();

}

void RenderSystem::update(double delta_t) {
    // clear pass
    _clear_shader->use();
    m_GBuffer->use();

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    _quad->draw();
    m_GBuffer->unuse();
    _clear_shader->unuse();

    // render all geometry components
    m_terrainGRenderStage->render();


    // lighting pass
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    _lighting_shader->use();
    m_GBuffer->read(_lighting_shader);
    m_CBuffer->read(_lighting_shader, 3);
    _quad->draw();
    _lighting_shader->unuse();
    m_GBuffer->unread();
    m_CBuffer->unread();

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
                _camera->key_callback(windowEvent.key);
                break;

            default:
                break;
            }
        }
}

RenderSystem::~RenderSystem() {


}
