#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <functional>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class FrameBuffer {
public:
    FrameBuffer(int width, int height, int layers = 0);
    void init();
    void init3d();
    void drawLayer(int layer);
    void initDepth();
    ~FrameBuffer();
    void use();
    void unuse(int window_width, int window_height);
    GLuint getTextureID();
    void bindTexture(GLuint pos);


private:
    GLuint _fbo;
    GLuint _texture_color;
    GLuint _texture_color_3d;
    GLuint _texture_depth;
    int _width, _height, _layers;
    std::function<void(const int)> _func;
};



#endif // FRAME_BUFFER_H
