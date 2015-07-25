
#ifndef TERRAIN_RENDER_STAGE_H
#define TERRAIN_RENDER_STAGE_H
#pragma once

#include <memory>
#include <vector>
#include "shader.h"
#include "gbuffer.h"
#include "cbuffer.h"
#include "context.h"
#include "camera.h"
#include "world_types.h"
#include "terrain_generator.h"
#include "marching_cube_mesher.h"
#include "grider.h"
#include "quad.h"

class TerrainRenderStage {
public:
    TerrainRenderStage();
    void init(std::shared_ptr<GBuffer> gbuffer, std::shared_ptr<CBuffer> cbuffer, RenderContext context, std::shared_ptr<Camera> camera);
    void render();
    void dispose();
    void generateTerrain();
private:
    std::unique_ptr<Shader> m_geometryShader;
    std::shared_ptr<Shader> m_colorShader;
    std::shared_ptr<GBuffer> m_GBuffer;
    std::shared_ptr<CBuffer> m_CBuffer;
    std::vector<Triangle> m_triangles;
    std::unique_ptr<TerrainGenerator> m_terrainGenerator;
    std::unique_ptr<MarchingCubeMesher> m_mcMesher;
    std::unique_ptr<Grider> m_grider;
    std::unique_ptr<Quad> m_quad;
    GLuint _vao;
    GLuint _vbo;

};

#endif