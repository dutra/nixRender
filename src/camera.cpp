
#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

void Camera::init() {
    m_cameraPos = glm::vec3(-5.0f, 10.0f, -5.0f);
    m_cameraTarget = glm::vec3(10.0f, 0.0f, 10.0f);
    refresh_view();

}

void Camera::refresh_view() {
    glm::vec3 cameraDirection = glm::normalize(m_cameraPos - m_cameraTarget);

    glm::vec3 up = glm::vec3(0.0, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    m_cameraUp = glm::cross(cameraDirection, cameraRight);

    m_view = glm::lookAt(m_cameraPos, m_cameraTarget, m_cameraUp);


}

glm::mat4 Camera::getViewMatrix() {
    return m_view;
}

void Camera::key_callback(sf::Event::KeyEvent event) {
    glm::vec3 cameraDirection = glm::normalize(m_cameraPos - m_cameraTarget);
    glm::vec3 up = glm::vec3(0.0, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

    switch (event.code) {
        case sf::Keyboard::W:
            m_cameraPos -= cameraDirection;
            m_cameraTarget -= cameraDirection;
            break;
        case sf::Keyboard::S:
            m_cameraPos += cameraDirection;
            m_cameraTarget += cameraDirection;
            break;
        case sf::Keyboard::A:
            m_cameraPos -= cameraRight;
            m_cameraTarget -= cameraRight;
            break;
        case sf::Keyboard::D:
            m_cameraPos += cameraRight;
            m_cameraTarget += cameraRight;
            break;
    }
    refresh_view();
}

void Camera::mouse_callback(sf::Event::MouseButtonEvent event) {
    glm::vec3 cameraDirection = glm::normalize(m_cameraPos - m_cameraTarget);
    glm::vec3 up = glm::vec3(0.0, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

}
