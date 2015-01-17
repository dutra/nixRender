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
#include "shader.h"
#include "quad.h"
#include "frame_buffer.h"

void check_events(sf::Window& window);
void setLayer(Shader shader, int layer);

int main() {
    std::cout << intro << "Initializing Rendering procedure" << std::endl;
    std::cout << reset << "==============================================" << std::endl;
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    sf::Window window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32), "OpenGL", sf::Style::Titlebar | sf::Style::Close, settings);
    // fps counter
    int frames_counter(0);
    double total_elapsed_secs(0.0);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    Quad quad;

    //////////////////////////////////////////////////////////////////////////
    // Transmittance
    Shader transmittanceShader("shaders/atmo/std.vert", "shaders/atmo/transmittance.frag", "shaders/atmo/common.glsl");
    transmittanceShader.bindFragDataLocation(0, "outColor");
    transmittanceShader.compile();
    FrameBuffer transmittanceFbo(TRANSMITTANCE_W, TRANSMITTANCE_H);
    transmittanceFbo.init();
    transmittanceShader.use();
    transmittanceFbo.use();
    quad.draw();
    
    transmittanceFbo.unuse(WINDOW_WIDTH, WINDOW_HEIGHT);
    transmittanceShader.unuse();
    

    //////////////////////////////////////////////////////////////////////////
    // Irradiance1
    Shader irradiance1Shader("shaders/atmo/std.vert", "shaders/atmo/irradiance1.frag", "shaders/atmo/common.glsl");
    irradiance1Shader.bindFragDataLocation(0, "outColor");
    irradiance1Shader.compile();
    
    FrameBuffer deltaEFbo(SKY_W, SKY_H);
    deltaEFbo.init();
    

    irradiance1Shader.use();
    deltaEFbo.use();

    irradiance1Shader.setTextureUniform("transmittanceSampler", 2);
    transmittanceFbo.bindTexture(2);
    quad.draw();

    deltaEFbo.unuse(WINDOW_WIDTH, WINDOW_HEIGHT);
    irradiance1Shader.unuse();

    //////////////////////////////////////////////////////////////////////////
    // IrradianceCopy
    Shader irradianceCopyShader("shaders/atmo/std.vert", "shaders/atmo/irradianceCopy.frag", "shaders/atmo/common.glsl");
    irradianceCopyShader.bindFragDataLocation(0, "outColor");
    irradianceCopyShader.compile();

    FrameBuffer irradianceFbo(SKY_W, SKY_H);
    irradianceFbo.init();

    irradianceCopyShader.use();
    irradianceFbo.use();

    glUniform1f(irradianceCopyShader.getUniformLocation("k"), 0.0);
    irradianceCopyShader.setTextureUniform("deltaESampler", 2);
    deltaEFbo.bindTexture(2);
    quad.draw();

    irradianceFbo.unuse(WINDOW_WIDTH, WINDOW_HEIGHT);
    irradianceCopyShader.unuse();
    std::cout << info << "E Texture: " << irradianceFbo.getTextureID() << std::endl;

    //////////////////////////////////////////////////////////////////////////
    // Inscatter1R
    // computes single scattering texture Rayleigh deltaSR (line 3 in algorithm 4.1)
    //////////////////////////////////////////////////////////////////////////////

    Shader inscatter1RShader("shaders/atmo/std.vert", "shaders/atmo/inscatter1R.frag", "shaders/atmo/common.glsl");
    inscatter1RShader.bindFragDataLocation(0, "outColor");
    inscatter1RShader.compile();


    FrameBuffer inscatterFboR(RES_MU_S * RES_NU, RES_MU, RES_R);
    inscatterFboR.init();
    inscatterFboR.init3d();
    inscatter1RShader.use();
    inscatter1RShader.setTextureUniform("transmittanceSampler", 3);
    for (int layer = 0; layer < RES_R; layer++) {
    
        inscatterFboR.use();
        transmittanceFbo.bindTexture(3);
        setLayer(inscatter1RShader, layer);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        quad.draw();
        inscatterFboR.drawLayer(layer);
        inscatterFboR.unuse(WINDOW_WIDTH, WINDOW_HEIGHT);

        std::cout << "Finished layer " << layer << std::endl;
        inscatterFboR.unuse(WINDOW_WIDTH, WINDOW_HEIGHT);
    }
    
    inscatter1RShader.unuse();
    
    //////////////////////////////////////////////////////////////////////////
    // Inscatter1M
    // computes single scattering texture Mie deltaSM (line 3 in algorithm 4.1)
    //////////////////////////////////////////////////////////////////////////////

    Shader inscatter1MShader("shaders/atmo/std.vert", "shaders/atmo/inscatter1M.frag", "shaders/atmo/common.glsl");
    inscatter1MShader.bindFragDataLocation(0, "outColor");
    inscatter1MShader.compile();


    FrameBuffer inscatterFboM(RES_MU_S * RES_NU, RES_MU, RES_R);
    inscatterFboM.init();
    inscatterFboM.init3d();
    inscatter1MShader.use();
    inscatter1MShader.setTextureUniform("transmittanceSampler", 3);
    for (int layer = 0; layer < RES_R; layer++) {

        inscatterFboM.use();
        transmittanceFbo.bindTexture(3);
        setLayer(inscatter1RShader, layer);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        quad.draw();
        inscatterFboM.drawLayer(layer);
        inscatterFboM.unuse(WINDOW_WIDTH, WINDOW_HEIGHT);

        std::cout << "Finished layer " << layer << std::endl;
        inscatterFboM.unuse(WINDOW_WIDTH, WINDOW_HEIGHT);
    }

    inscatter1MShader.unuse();

    //////////////////////////////////////////////////////////////////////////
    // Inscatter1Copy
    // copies deltaS into inscatter texture S (line 5 in algorithm 4.1)
    //////////////////////////////////////////////////////////////////////////////

    Shader inscatter1CopyShader("shaders/atmo/std.vert", "shaders/atmo/inscatter1Copy.frag", "shaders/atmo/common.glsl");
    inscatter1CopyShader.bindFragDataLocation(0, "outColor");
    inscatter1CopyShader.compile();


    FrameBuffer inscatterFbo1(RES_MU_S * RES_NU, RES_MU, RES_R);
    inscatterFbo1.init();
    inscatterFbo1.init3d();
    inscatter1CopyShader.use();
    inscatter1CopyShader.setTextureUniform("deltaSRSampler", 10);
    inscatter1CopyShader.setTextureUniform("deltaSMSampler", 11);
    
    glDisable(GL_BLEND);
    for (int layer = 0; layer < RES_R; layer++) {

        inscatterFbo1.use();
        inscatterFboR.bindTexture3d(10);
        inscatterFboM.bindTexture3d(11);
        glUniform1i(inscatter1CopyShader.getUniformLocation("layer"), layer);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        quad.draw();
        inscatterFbo1.drawLayer(layer);
        inscatterFbo1.unuse(WINDOW_WIDTH, WINDOW_HEIGHT);

        std::cout << "Finished layer " << layer << std::endl;
        inscatterFbo1.unuse(WINDOW_WIDTH, WINDOW_HEIGHT);
    }

    inscatter1CopyShader.unuse();




    ////////////////////////////////////////////////////////////////////////
    

    while (window.isOpen()) {

        check_events(window);

        // Initialize counter
        std::clock_t begin = std::clock();


        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        

        // Swap buffers
        window.display();

        std::clock_t end = std::clock();
        total_elapsed_secs += double(end - begin) / CLOCKS_PER_SEC;
        if (++frames_counter == FRAMES_COUNTER) {
            std::cout << reset << "Avg. drawing time: " << blue << total_elapsed_secs / FRAMES_COUNTER * 1000 << reset << " ms, FPS: " << blue << 1.0 / (total_elapsed_secs / FRAMES_COUNTER) << reset << "." << std::endl;
            frames_counter = 0;
            total_elapsed_secs = 0;
        }
    }

}

void setLayer(Shader shader, int layer) {
    double r = layer / (RES_R - 1.0);
    r = r * r;
    r = sqrt(Rg * Rg + r * (Rt * Rt - Rg * Rg)) + (layer == 0 ? 0.01 : (layer == RES_R - 1 ? -0.001 : 0.0));
    double dmin = Rt - r;
    double dmax = sqrt(r * r - Rg * Rg) + sqrt(Rt * Rt - Rg * Rg);
    double dminp = r - Rg;
    double dmaxp = sqrt(r * r - Rg * Rg);
    glUniform1f(shader.getUniformLocation("r"), float(r));
    glUniform4f(shader.getUniformLocation("dhdH"), float(dmin), float(dmax), float(dminp), float(dmaxp));
    
        
}

void check_events(sf::Window& window) {
    sf::Event windowEvent;
    while (window.pollEvent(windowEvent)) {
        switch (windowEvent.type) {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::KeyPressed:
            if (windowEvent.key.code == sf::Keyboard::Escape)
                window.close();
            break;
        default:
            break;
        }
    }
}