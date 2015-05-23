#include "shader.h"
#include <string>
#include <fstream>
#include <cassert>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "color.h"
#include "main.h"
#include "error.h"

Shader::Shader(std::string path_vert, std::string path_frag, std::string path_common) {

    //std::cout << reset << "Loading Vertex Shader " << path_vert << std::endl;

    if (path_common.length() > 0) { // test if common shader exists
        std::ifstream input_common(path_common);

        if (!input_common) {
            std::cout << error << "ERROR: Failure to open common shader file " << path_common << std::endl;
            assert(false);
        }
    }
    std::ifstream input_shader_vert(path_vert);
    if (!input_shader_vert) { // test if vertice shader exists
        std::cout << error << "ERROR: Failure to open vertice shader " << path_vert << std::endl;
        assert(false);
    }

    std::stringstream str_stream_vert;
    str_stream_vert << "#version " << GLSL_VERSION << "\n";
    if (path_common.length() > 0) {
        std::ifstream input_common(path_common);
        //str_stream_vert << input_common.rdbuf();
    }
    str_stream_vert << input_shader_vert.rdbuf();
    _shader_vert_src = str_stream_vert.str();

    //std::cout << reset << "Loading Fragment Shader " << path_frag << std::endl;
    std::ifstream input_shader_frag(path_frag);
    if (!input_shader_frag) {
        std::cout << error << "ERROR: Failure to open frag shader " << path_frag << std::endl;
        assert(false);
    }
    std::stringstream str_stream_frag;
    str_stream_frag << "#version " << GLSL_VERSION << "\n";
    if (path_common.length() > 0) {
        std::ifstream input_common(path_common);
        str_stream_frag << input_common.rdbuf();
    }
    str_stream_frag << input_shader_frag.rdbuf();

    _shader_frag_src = str_stream_frag.str();
    glCheckError();

}

void Shader::compile() {

    //std::cout << reset << "Compiling Shader" << std::endl;

    const char *vert_src_c = _shader_vert_src.c_str();
    const char *frag_src_c = _shader_frag_src.c_str();

    // Create and compile the vertex shader
    _vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(_vertex_shader, 1, &vert_src_c, NULL);
    glCompileShader(_vertex_shader);

    GLint isCompiled = 0;
    glGetShaderiv(_vertex_shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(_vertex_shader, GL_INFO_LOG_LENGTH, &maxLength);

        //The maxLength includes the NULL character
        std::vector<char> errorLog(maxLength);
        glGetShaderInfoLog(_vertex_shader, maxLength, &maxLength, &errorLog[0]);
        std::string error_log_str(errorLog.begin(), errorLog.end());
        std::cout << red << "ERROR: Compiling Vertex Shader " << error_log_str << std::endl;

        //Exit with failure.
        glDeleteShader(_vertex_shader); //Don't leak the shader.
        assert(false);
    }

    // Create and compile the fragment shader
    _fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(_fragment_shader, 1, &frag_src_c, NULL);
    glCompileShader(_fragment_shader);

    glGetShaderiv(_fragment_shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(_fragment_shader, GL_INFO_LOG_LENGTH, &maxLength);

        //The maxLength includes the NULL character
        std::vector<char> errorLog(maxLength);
        glGetShaderInfoLog(_fragment_shader, maxLength, &maxLength, &errorLog[0]);

        std::string error_log_str(errorLog.begin(), errorLog.end());

        std::cout << red << "ERROR: Compiling FRAGMENT Shader:" << std::endl << error_log_str << std::endl;

        //Exit with failure.
        glDeleteShader(_fragment_shader); //Don't leak the shader.
        assert(false);
    }

    // Link the vertex and fragment shader into a shader program
    _shader_program = glCreateProgram();
    glAttachShader(_shader_program, _vertex_shader);
    glAttachShader(_shader_program, _fragment_shader);
    glCheckError();

    for (auto m : _fragDataLocation) {
        glBindFragDataLocation(_shader_program, m.first, m.second.c_str());
        glCheckError();
        //std::cout << reset << "Binding FragDataLocation " << m.second << std::endl;
    }

    glLinkProgram(_shader_program);

    glCheckError();
    //std::cout << info << "Done compiling Shader" << std::endl;
}


void Shader::use() {
    glCheckError();
    glUseProgram(_shader_program);
    glCheckError();
}

void Shader::setUniforms() {

}

void Shader::unuse() {
    glUseProgram(0);
    glCheckError();
}

Shader::~Shader() {
    glDeleteProgram(_shader_program);
    glDeleteShader(_vertex_shader);
    glDeleteShader(_fragment_shader);
    glCheckError();
}


GLuint Shader::getProgram() {
    return _shader_program;
}


void Shader::bindFragDataLocation(int i, std::string name) {
    _fragDataLocation[i] = name;
    //std::cout << reset << "Adding FragDataLocation " << name << " to output " << i << std::endl;

}

void Shader::setTextureUniform(std::string name, GLuint value) {

    GLint loc = glGetUniformLocation(getProgram(), name.c_str());
    GLint glerror = glGetError();
    if (glerror != GL_NO_ERROR) {
        std::cout << error << "ERROR: Shader::setTextureUniform glGetError" << std::endl;

        switch (glerror) {
        case GL_INVALID_ENUM:
            std::cout << "An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag." << std::endl;
            break;
        case GL_INVALID_VALUE:
            std::cout << "A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag." << std::endl;
            break;
        case GL_INVALID_OPERATION:
            std::cout << "The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag." << std::endl;
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            std::cout << "The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag." << std::endl;
            break;
        case GL_OUT_OF_MEMORY:
            std::cout << "There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded." << std::endl;
            break;
        case GL_STACK_UNDERFLOW:
            std::cout << "An attempt has been made to perform an operation that would cause an internal stack to underflow." << std::endl;
            break;
        case GL_STACK_OVERFLOW:
            std::cout << "An attempt has been made to perform an operation that would cause an internal stack to overflow." << std::endl;
            break;
        }
            assert(false);
    }

    if (loc == -1) {
        std::cout << error << "ERROR: Texture uniform " << name << " could not be found." << std::endl;
        return;
        assert(false);
    }
    glProgramUniform1i(getProgram(), loc, value);


}

GLint Shader::getUniformLocation(std::string name) {

    GLint loc = glGetUniformLocation(getProgram(), name.c_str());
//     if (loc == -1) {
//         std::cout << error << "ERROR: Uniform " << name << " could not be found." << std::endl;
//         assert(false);
//     }

    return loc;

}
