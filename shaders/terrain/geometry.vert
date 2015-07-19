
layout (location = 0) in vec3 vPosition; 
layout (location = 1) in vec3 vNormal; 


uniform mat4 unWorld;
uniform mat4 unView;
uniform mat4 unProj;

out vec3 fPosition; 
out vec3 fNormal; 

void main() { 
    gl_Position = unProj * unView * unWorld * vec4(vPosition, 1.0);
    fPosition = (unWorld * vec4(vPosition, 1.0)).xyz;
    fNormal = (unWorld * vec4(vNormal, 0.0)).xyz;
    
}