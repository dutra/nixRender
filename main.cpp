#include <iostream>
#include <ctime>
#include <fstream>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "main.h"
#include "color.h"

int main() {
  std::cout << intro << "Initializing Rendering procedure" << std::endl;

  sf::ContextSettings settings;
  settings.depthBits = 24;
  settings.stencilBits = 8;
  sf::Window window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32), "OpenGL", sf::Style::Titlebar | sf::Style::Close, settings);
  // fps counter
  int frames_counter{ 0 };
  double total_elapsed_secs{ 0.0 };

  // Initialize GLEW
  glewExperimental = GL_TRUE;
  glewInit();

  while (window.isOpen()) {

    // Initialize counter
    std::clock_t begin = std::clock();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Swap buffers
    window.display();
    
    std::clock_t end = std::clock();
    total_elapsed_secs += double(end - begin) / CLOCKS_PER_SEC;
    if (++frames_counter == FRAMES_COUNTER) {
      std::cout << reset << "Avg. drawing time: " << blue << total_elapsed_secs / 100.0 * 1000 << reset << " ms, FPS: " << blue << 1.0 / (total_elapsed_secs / 100.0) << std::endl;
      frames_counter = 0;
      total_elapsed_secs = 0;
    }
  }


}
