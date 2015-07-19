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
#include "chunk_manager.h"
#include "types.h"
#include "gbuffer.h"
#include "cbuffer.h"
#include "quad.h"
#include "camera.h"
#include "terrain_render_stage.h"
#include "context.h"

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
 	std::shared_ptr<Shader> _lighting_shader;
    std::shared_ptr<Shader> _pass_shader;
    std::shared_ptr<Shader> _simple_shader;
    std::unique_ptr<ChunkManager> _chunkManager;
    std::shared_ptr<GBuffer> m_GBuffer;
    std::shared_ptr<CBuffer> m_CBuffer;

    std::unique_ptr<Quad> _quad;
    std::shared_ptr<Camera> _camera;
    std::unique_ptr<TerrainRenderStage> m_terrainGRenderStage;
    RenderContext m_renderContext;
};


#endif
