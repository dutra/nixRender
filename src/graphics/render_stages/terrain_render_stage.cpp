
#include "terrain_render_stage.h"

#include <glm/gtc/type_ptr.hpp>
#include "error.h"

TerrainRenderStage::TerrainRenderStage() {
    m_geometryShader.reset(new Shader("shaders/terrain/geometry.vert", "shaders/terrain/geometry.frag"));
    m_colorShader.reset(new Shader("shaders/terrain/color.vert", "shaders/terrain/color.frag"));
    m_terrainGenerator.reset(new TerrainGenerator);
    m_mcMesher.reset(new MarchingCubeMesher);
    m_quad.reset(new Quad);
}


void TerrainRenderStage::init(std::shared_ptr<GBuffer> gbuffer, std::shared_ptr<CBuffer> cbuffer, RenderContext context, std::shared_ptr<Camera> camera) {

    m_GBuffer = gbuffer;
    m_CBuffer = cbuffer;

    // Geometry Shader
    m_geometryShader->bindFragDataLocation(0, "pPosition");
    m_geometryShader->bindFragDataLocation(1, "pNormal");
    m_geometryShader->bindFragDataLocation(2, "pIdentifier");
    m_geometryShader->compile();
    m_geometryShader->attachCamera(camera);
    m_geometryShader->use();
    GLint unProj = m_geometryShader->getUniformLocation("unProj");
    glUniformMatrix4fv(unProj, 1, GL_FALSE, glm::value_ptr(context.projection));
    GLint unWorld = m_geometryShader->getUniformLocation("unWorld");
    glUniformMatrix4fv(unWorld, 1, GL_FALSE, glm::value_ptr(context.world));
    m_geometryShader->unuse();

    // Color Shader
    m_colorShader->bindFragDataLocation(0, "pAmbient");
    m_colorShader->bindFragDataLocation(1, "pDiffuse");
    m_colorShader->bindFragDataLocation(2, "pSpecular");
    m_colorShader->compile();

    m_terrainGenerator->init();
    
    m_mcMesher->init([=](glm::vec3 position) {
        return m_terrainGenerator->getDensity(position);
    });

    m_quad->init();
}

void TerrainRenderStage::generateTerrain() {

    m_triangles = m_mcMesher->generateTriagles();


    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle) * m_triangles.size(), &m_triangles.front(), GL_STATIC_DRAW);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    //GLint posAttrib = glGetAttribLocation(_shader->getProgram(), "position");
    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, // index
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized
        sizeof(VertexNormal), // stride (byte offset between consecutive generic vertex attributes)
        0); // pointer (offset of the first component of the first generic vertex attribute)

    // normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, // index
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized
        sizeof(VertexNormal), // stride (byte offset between consecutive generic vertex attributes)
        (void*)(sizeof(glm::vec3))); // pointer (offset of the first component of the first generic vertex attribute)



}


void TerrainRenderStage::render() {

    m_geometryShader->use();
    
    m_GBuffer->use();
    
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(_vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glDrawArrays(GL_TRIANGLES, 0, m_triangles.size()*3);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    
    m_GBuffer->unuse();
    m_geometryShader->unuse();

    m_colorShader->use();
    m_GBuffer->read(m_colorShader);
    m_CBuffer->use();
    glDisable(GL_DEPTH_TEST);
    m_quad->draw();
    m_CBuffer->unuse();
    m_GBuffer->unread();
    m_colorShader->unuse();


    glCheckError();
}


void TerrainRenderStage::dispose() {

}
