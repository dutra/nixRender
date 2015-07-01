#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <iostream>
#include <memory>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>


#include "color.h"
#include "shader.h"
#include "frame_buffer.h"
#include "chunk_manager.h"
#include "types.h"
#include "gbuffer.h"
#include "cube.h"
#include "quad.h"

class RenderSystem {
public:
    RenderSystem();
    void init();
    void update(double delta_t);
    bool isWindowOpen();


    ~RenderSystem();
private:
    uint64 _frames_counter;
    double _t_total;
    std::unique_ptr<sf::ContextSettings> _settings;
    std::unique_ptr<sf::Window> _window;
    void check_events();
    void countFPS(double delta_t);
    //std::shared_ptr<Shader> _blockShader;
    std::shared_ptr<Shader> _geometry_shader;
	std::shared_ptr<Shader> _lighting_shader;
	std::shared_ptr<Shader> _pass_shader;
	std::shared_ptr<Shader> _luma_shader;
	std::shared_ptr<Shader> _gaussian_first_shader, _gaussian_second_shader;
	std::unique_ptr<ChunkManager> _chunkManager;
    std::unique_ptr<GBuffer> _gbuffer;
	std::unique_ptr<FrameBuffer> _fbo, _fbo_luma, _fbo_gaussian_first, _fbo_gaussian_second;
	glm::mat4 _view, _proj, _world;
    glm::vec3 _view_pos;
    std::unique_ptr<Cube> _cube, _cube2, _cube3;
    std::unique_ptr<Quad> _quad;
};


#endif
