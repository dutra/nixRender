
layout (location = 0) in vec3 Position; 
layout (location = 1) in vec3 Normal; 
layout (location = 2) in vec2 TexCoord; 


uniform mat4 world;
uniform mat4 view;
uniform mat4 proj;

//uniform vec3 diffuse;

out vec3 WorldPos0; 
out vec3 Normal0; 
out vec2 TexCoord0; 


void main() { 
    gl_Position = proj * view * world * vec4(Position, 1.0);
    //gl_Position = vec4(Position, 1.0);
    WorldPos0 = (world * vec4(Position, 1.0)).xyz;
    Normal0 = (world * vec4(Normal, 0.0)).xyz;
    TexCoord0 = TexCoord;
    
}