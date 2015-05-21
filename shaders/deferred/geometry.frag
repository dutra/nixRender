in vec2 TexCoord0; 
in vec3 Normal0; 
in vec3 WorldPos0; 

out vec3 OutWorldPos; 
out vec3 OutDiffuse; 
out vec3 OutNormal; 
out vec3 OutTexCoord; 

//uniform sampler2D gColorMap; 

void main() 
{ 
    OutWorldPos = WorldPos0;
    //OutWorldPos = vec3(0.0, 1.0, 0.0);
    //DiffuseOut = texture(gColorMap, TexCoord0).xyz; 
    OutDiffuse = vec3(0.0, 0.0, 1.0);
    //NormalOut = normalize(Normal0); 
    OutNormal = vec3(1.0, 1.0, 0.0);
    //TexCoordOut = vec3(TexCoord0, 0.0); 
    OutTexCoord = vec3(1.0, 0.0, 0.0);
}