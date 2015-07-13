
#ifndef CAMERA_H
#define CAMERA_H
#pragma once

#include <glm/glm.hpp>
#include <SFML/Window/Event.hpp>

class Camera {
public:
    void init();
    void key_callback(sf::Event::KeyEvent event);
    void mouse_callback(sf::Event::MouseButtonEvent event);
    glm::mat4 getViewMatrix();

private:
    void refresh_view();

    glm::vec3 m_cameraPos;
    glm::vec3 m_cameraTarget;
    glm::vec3 m_cameraUp;
    glm::mat4 m_view;
};

#endif