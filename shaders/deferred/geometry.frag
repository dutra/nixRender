in vec3 WorldPos0; 
in vec3 Normal0; 

out vec3 OutWorldPos; 
out vec3 OutNormal; 

void main() { 

    OutWorldPos = WorldPos0;    
    OutNormal = normalize(Normal0);
    
}