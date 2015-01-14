#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>
#include <map>

class Shader {
public:
    Shader(std::string path_vert, std::string path_frag, std::string path_common = std::string());
    void compile();
    GLuint getProgram();
    void use();
    void unuse();
    void setUniforms();
    GLint getUniformLocation(std::string name);
    void bindFragDataLocation(int i, std::string name);
    void setTextureUniform(std::string name, GLuint value);
    ~Shader();

private:
    std::string _shader_vert_src;
    std::string _shader_frag_src;
    GLuint _vertex_shader;
    GLuint _fragment_shader;
    GLuint _shader_program;
    std::map<int, std::string> _fragDataLocation;
};

#endif // SHADER_H
