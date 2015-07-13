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

float gauss(int i, float sigma2) {
	return 1.0 / std::sqrt(2 * 3.14159265 * sigma2) * std::exp(-(i*i) / (2 * sigma2));
}


RenderSystem::RenderSystem() {
    _settings.reset(new sf::ContextSettings());
    _settings->depthBits = 24;
    _settings->stencilBits = 8;
	_settings->antialiasingLevel = 8;
    _frames_counter = 0;
    _t_total = 0.0;
    _window.reset(new sf::Window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32), "OpenGL", sf::Style::Titlebar | sf::Style::Close, *_settings));
//    _blockShader.reset(new Shader("shaders/block.vert", "shaders/block.frag"));
    _clear_shader.reset(new Shader("shaders/deferred/clear.vert", "shaders/deferred/clear.frag"));
    _geometry_shader.reset(new Shader("shaders/deferred/geometry.vert", "shaders/deferred/geometry.frag"));
    _lighting_shader.reset(new Shader("shaders/deferred/lighting.vert", "shaders/deferred/lighting.frag"));
	_pass_shader.reset(new Shader("shaders/pass.vert", "shaders/pass.frag"));
    _chunkManager.reset(new ChunkManager());
	_quad.reset(new Quad);
	_gbuffer.reset(new GBuffer(WINDOW_WIDTH, WINDOW_HEIGHT));
    _mcm.reset(new MarchingCubeMesher);
}

void RenderSystem::init() {
    std::cout << intro << "Initializing Rendering procedure" << std::endl;
    std::cout << reset << "==============================================" << std::endl;

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();

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
    _clear_shader->bindFragDataLocation(2, "OutDiffuse");
    _clear_shader->bindFragDataLocation(3, "OutTexCoord");
    _clear_shader->compile();

    // geometry shader
    _geometry_shader->bindFragDataLocation(0, "OutWorldPos");
    _geometry_shader->bindFragDataLocation(1, "OutNormal");
    _geometry_shader->bindFragDataLocation(2, "OutDiffuse");
    _geometry_shader->bindFragDataLocation(3, "OutTexCoord");
    _geometry_shader->compile();
    _geometry_shader->use();

    _gbuffer->init();

	// Set up view
    _view_pos = glm::vec3(-5.0f, 10.0f, -5.0f);
    _view = glm::lookAt(
    _view_pos, // eye
    glm::vec3(10.0f, 0.0f, 10.0f), // center
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

    // Set up diffuse
    glm::vec3 diffuse = glm::vec3(1.0, 1.0, 1.0);
    GLint uni_diffuse = _geometry_shader->getUniformLocation("diffuse");
    glUniform3fv(uni_diffuse, 1, glm::value_ptr(diffuse));

    _geometry_shader->unuse();

    _chunkManager->init();
    _quad->init();

    _mcm->init();

    glCheckError();

}

void RenderSystem::update(double delta_t) {
    // clear pass
    _clear_shader->use();
    _gbuffer->use();
    glDisable(GL_DEPTH_TEST);
    _quad->draw();
    _gbuffer->unuse();
    _clear_shader->unuse();

    // geometry pass
    _geometry_shader->use();
    _gbuffer->use();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    GLint uniView = _geometry_shader->getUniformLocation("view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(_view));

    // render all components
    _chunkManager->render();
    _mcm->render();
	_gbuffer->unuse();
    _geometry_shader->unuse();

    // lighting pass
    glDisable(GL_DEPTH_TEST);
    _lighting_shader->use();
    _gbuffer->read(_lighting_shader);
    _quad->draw();
    _lighting_shader->unuse();
    _gbuffer->unread();

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
                if (windowEvent.key.code == sf::Keyboard::W) {
                    _view_pos += glm::vec3(0.0f, 0.1f, 0.0f);
                    _view = glm::lookAt(
                        _view_pos, // eye
                        glm::vec3(0.0f, 0.0f, 0.0f), // center
                        glm::vec3(0.0f, 1.0f, 0.0f) // up
                        );
                }
                if (windowEvent.key.code == sf::Keyboard::S) {
                    _view_pos += glm::vec3(0.0f, -0.1f, 0.0f);
                    _view = glm::lookAt(
                        _view_pos, // eye
                        glm::vec3(0.0f, 0.0f, 0.0f), // center
                        glm::vec3(0.0f, 1.0f, 0.0f) // up
                        );
                }
                if (windowEvent.key.code == sf::Keyboard::A) {
                    _view_pos += glm::vec3(-0.1f, 0.0f, 0.0f);
                    _view = glm::lookAt(
                        _view_pos, // eye
                        glm::vec3(0.0f, 0.0f, 0.0f), // center
                        glm::vec3(0.0f, 1.0f, 0.0f) // up
                        );
                }
                if (windowEvent.key.code == sf::Keyboard::D) {
                    _view_pos += glm::vec3(0.1f, 0.0f, 0.0f);
                    _view = glm::lookAt(
                        _view_pos, // eye
                        glm::vec3(0.0f, 0.0f, 0.0f), // center
                        glm::vec3(0.0f, 1.0f, 0.0f) // up
                        );
                }
                if (windowEvent.key.code == sf::Keyboard::LShift) {
                    _view_pos += glm::vec3(0.0f, 0.0f, -0.1f);
                    _view = glm::lookAt(
                        _view_pos, // eye
                        glm::vec3(0.0f, 0.0f, 0.0f), // center
                        glm::vec3(0.0f, 1.0f, 0.0f) // up
                        );
                }
                if (windowEvent.key.code == sf::Keyboard::LControl) {
                    _view_pos += glm::vec3(0.0f, 0.0f, 0.1f);
                    _view = glm::lookAt(
                        _view_pos, // eye
                        glm::vec3(0.0f, 0.0f, 0.0f), // center
                        glm::vec3(0.0f, 1.0f, 0.0f) // up
                        );
                }
                break;

            default:
                break;
            }
        }
}

RenderSystem::~RenderSystem() {


}
