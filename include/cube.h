
#ifndef CUBE_H
#define CUBE_H

#include <memory>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "world_types.h"
#include "shader.h"
#include "texture.h"
#include "shader.h"

class Cube {

public:
    Cube(std::string color_map, std::string normal_map);
    ~Cube();

    void init();
    void recreate_blocks();
    void render(std::shared_ptr<Shader> shader);
	void translate(glm::vec3 pos);
	void scale(glm::vec3 factor);

private:
    GLuint _vao;
    GLuint _vbo;
    std::vector<VertexNormalUV> _vertices;
    std::unique_ptr<Texture> _texture_color, _texture_normal;
	glm::mat4 _model;
};

#endif
