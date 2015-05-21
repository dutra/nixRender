
layout (location = 0) in vec3 Position; 
layout (location = 1) in vec2 TexCoord; 
layout (location = 2) in vec3 Normal; 

uniform mat4 world;
uniform mat4 view;
uniform mat4 proj;

out vec2 TexCoord0; 
out vec3 Normal0; 
out vec3 WorldPos0; 

void main()
{ 
    gl_Position = proj * view * world * vec4(Position, 1.0);
    TexCoord0 = TexCoord; 
    Normal0 = (world * vec4(Normal, 0.0)).xyz; 
    WorldPos0 = (world * vec4(Position, 1.0)).xyz;
}