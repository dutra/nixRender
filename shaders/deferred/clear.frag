
in vec2 TexCoord0; 

out vec4 OutWorldPos; 
out vec4 OutNormal; 
out vec4 OutDiffuse; 
out vec4 OutTexCoord; 

void main() { 
    OutWorldPos = vec4(1.0, 0.0, 0.0, 1.0);
    OutNormal = vec4(0.0, 1.0, 0.0, 1.0);
    OutDiffuse = vec4(0.0, 0.0, 1.0, 1.0);
    OutTexCoord = vec4(0.0, 0.0, 0.0, 1.0);
}

