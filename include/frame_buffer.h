#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class FrameBuffer {
public:
    FrameBuffer(int window_width, int window_height);
    void init();
    void initDepth();
    ~FrameBuffer();
    void use();
    void unuse();

private:
    GLuint _fbo;
    GLuint _texture_color;
    GLuint _texture_depth;
    int _width, _height;
};



#endif // FRAME_BUFFER_H