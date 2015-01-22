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

class RenderSystem {
public:
    RenderSystem();
    void init();
    void update(double delta_t);
    bool isWindowOpen();


    ~RenderSystem();
private:
    int _frames_counter;
    double _total_elapsed_secs;
    std::unique_ptr<sf::ContextSettings> _settings;
    std::unique_ptr<sf::Window> _window;
    void check_events();
    void countFPS(double delta_t);
};


#endif
