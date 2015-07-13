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
#include "marching_cube_mesher.h"

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
    std::shared_ptr<Shader> _clear_shader;
    std::shared_ptr<Shader> _geometry_shader;
	std::shared_ptr<Shader> _lighting_shader;
	std::shared_ptr<Shader> _pass_shader;
	std::unique_ptr<ChunkManager> _chunkManager;
    std::unique_ptr<GBuffer> _gbuffer;
	glm::mat4 _view, _proj, _world;
    glm::vec3 _view_pos;
    std::unique_ptr<Quad> _quad;
    std::unique_ptr<MarchingCubeMesher> _mcm;
};


#endif
