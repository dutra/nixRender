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
    _geometry_shader.reset(new Shader("shaders/deferred/geometry.vert", "shaders/deferred/geometry.frag"));
	_lighting_shader.reset(new Shader("shaders/deferred/lighting.vert", "shaders/deferred/lighting.frag"));
	_pass_shader.reset(new Shader("shaders/pass.vert", "shaders/pass.frag"));
	_luma_shader.reset(new Shader("shaders/bloom/luma.vert", "shaders/bloom/luma.frag"));
	_gaussian_first_shader.reset(new Shader("shaders/bloom/first.vert", "shaders/bloom/first.frag"));
	_gaussian_second_shader.reset(new Shader("shaders/bloom/second.vert", "shaders/bloom/second.frag"));

	_cube.reset(new Cube("assets/stone/color.png", "assets/stone/normal.png"));
	_cube2.reset(new Cube("assets/stone/color.png", "assets/stone/normal.png"));
	_cube3.reset(new Cube("assets/white.png", "assets/white.png"));

	_quad.reset(new Quad);
	_gbuffer.reset(new GBuffer(WINDOW_WIDTH, WINDOW_HEIGHT));
	_fbo.reset(new FrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT));
	_fbo_luma.reset(new FrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT));
	_fbo_gaussian_first.reset(new FrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT));
	_fbo_gaussian_second.reset(new FrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT));
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
	//	_pass_shader->use();

	// luma
	_luma_shader->bindFragDataLocation(0, "TexCoord0");
	_luma_shader->compile();
	_luma_shader->use();
//	GLint uniLumThresh = _luma_shader->getUniformLocation("LumThresh");
//	glUniform1f(uniLumThresh, 0.75);
	_luma_shader->unuse();

	// gaussian shader
	_gaussian_first_shader->bindFragDataLocation(0, "TexCoord0");
	_gaussian_first_shader->compile();
	_gaussian_first_shader->use();
	GLint uniHeight = _gaussian_first_shader->getUniformLocation("Height");
	glUniform1i(uniHeight, WINDOW_HEIGHT);
	_gaussian_first_shader->unuse();

	_gaussian_second_shader->bindFragDataLocation(0, "TexCoord0");
	_gaussian_second_shader->compile();
	_gaussian_second_shader->use();
	GLint uniWidth = _gaussian_second_shader->getUniformLocation("Width");
	glUniform1i(uniHeight, WINDOW_WIDTH);
	_gaussian_second_shader->unuse();

	// calculate gaussian weights
	float weights[20], sum, sigma2 = 25.0f;
	weights[0] = gauss(0, sigma2);
	sum = weights[0];
	for (int i = 0; i < 20; i++) {
		weights[i] = gauss(i, sigma2);
		sum += 2 * weights[i];
	}
	for (int i = 0; i < 20; i++) {
		std::ostringstream oss;
		oss << "Weight[";
		oss << i;
		oss << "]";
		std::string weight_name = oss.str();
		std::cout << weight_name << ": " << weights[i] / sum << std::endl;

		_gaussian_first_shader->use();
		glUniform1f(_gaussian_first_shader->getUniformLocation(weight_name), weights[i] / sum);
		_gaussian_first_shader->unuse();
		_gaussian_second_shader->use();
		glUniform1f(_gaussian_second_shader->getUniformLocation(weight_name), weights[i] / sum);
		_gaussian_second_shader->unuse();
	}

	



	// fbo
	_fbo->init(GL_RGBA16F, GL_RGBA, GL_FLOAT);
	_fbo_luma->init(GL_RGBA16F, GL_RGBA, GL_FLOAT);
	_fbo_gaussian_first->init(GL_RGBA16F, GL_RGBA, GL_FLOAT);
	_fbo_gaussian_second->init(GL_RGBA16F, GL_RGBA, GL_FLOAT);



    // geometry shader
    _geometry_shader->bindFragDataLocation(0, "OutWorldPos");
    _geometry_shader->bindFragDataLocation(1, "OutNormal");
    _geometry_shader->bindFragDataLocation(2, "OutDiffuse");
    _geometry_shader->bindFragDataLocation(3, "OutTexCoord");
    _geometry_shader->compile();
    _geometry_shader->use();

    _gbuffer->init();

	// Set up view
    _view_pos = glm::vec3(-2.0f, -1.0f, 1.0f);
    _view = glm::lookAt(
    _view_pos, // eye
    glm::vec3(0.0f, 0.0f, 1.0f), // center
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

    _cube->init();
	_cube2->init();
	_cube2->translate(glm::vec3(0, 0.0, 1.2));
	_cube3->init();
	_cube3->translate(glm::vec3(2.0, 0.0, 0.0));
	_cube3->scale(glm::vec3(5.0));

    _geometry_shader->unuse();

//    _lighting_shader->use();
    _quad->init();

    glCheckError();

	std::cout << "FBO Texture ID: " << _fbo->getTextureID() << std::endl;
	std::cout << "Luma FBO Texture ID: " << _fbo_luma->getTextureID() << std::endl;
	std::cout << "Gaussian First FBO Texture ID: " << _fbo_gaussian_first->getTextureID() << std::endl;
	std::cout << "Gaussian Second FBO Texture ID: " << _fbo_gaussian_second->getTextureID() << std::endl;

}

void RenderSystem::update(double delta_t) {
    // geometry pass
    _geometry_shader->use();
    _gbuffer->use();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    // render all components
    //_chunkManager->render();

    GLint uniView = _geometry_shader->getUniformLocation("view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(_view));

	_cube->render(_geometry_shader);
	_cube2->render(_geometry_shader);
	_cube3->render(_geometry_shader);
	_gbuffer->unuse();
    _geometry_shader->unuse();


    // lighting pass
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    _lighting_shader->use();
	_fbo->use();
    _gbuffer->read(_lighting_shader);
    _quad->draw();
	_fbo->unuse();
    _lighting_shader->unuse();

	// post processing
	// bloom first pass -- luminance
	_luma_shader->use();
	_fbo->bindTexture(0);
	_fbo_luma->use();
	_luma_shader->setTextureUniform("colorTex", 0);
	_quad->draw();
	_fbo_luma->unuse();
	_luma_shader->unuse();

	// bloom second pass -- y blur
	_gaussian_first_shader->use();
	_fbo_luma->bindTexture(0);
	_fbo_gaussian_first->use();
	_gaussian_first_shader->setTextureUniform("BlurTex", 0);
	_quad->draw();
	_fbo_gaussian_first->unuse();
	_gaussian_first_shader->unuse();

	// bloom third pass -- x blur
	_gaussian_second_shader->use();
	_fbo->bindTexture(0);
	_gaussian_second_shader->setTextureUniform("RenderTex", 0);
	_fbo_gaussian_first->bindTexture(1);
	_gaussian_second_shader->setTextureUniform("BlurTex", 1);
	_fbo_gaussian_second->use();
	_quad->draw();
	_fbo_gaussian_second->unuse();
	_gaussian_second_shader->unuse();


	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_pass_shader->use();
	_fbo_gaussian_second->bindTexture(0);
	_pass_shader->setTextureUniform("colorTex", 0);
	_quad->draw();
	_pass_shader->unuse();

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
