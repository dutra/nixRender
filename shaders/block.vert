
in vec3 position;

//out vec3 Color;
//out vec2 Texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    //Color = color;
//    Texcoord = texcoord;
    gl_Position = proj * view * model * vec4(position, 1.0);
}
