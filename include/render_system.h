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
    std::shared_ptr<Shader> _blockShader;
    std::unique_ptr<ChunkManager> _chunkManager;

    glm::mat4 _view, _proj, _model;
};


#endif
