in vec3 WorldPos0; 
in vec3 Normal0; 
in vec2 TexCoord0; 

out vec3 OutWorldPos; 
out vec3 OutNormal; 
out vec3 OutDiffuse; 
out vec3 OutTexCoord; 


uniform sampler2D colorMap;
uniform sampler2D normalMap;

vec3 diffuseColor = vec3(1.0, 1.0, 1.0);

//uniform sampler2D gColorMap; 

void main() { 
    OutWorldPos = WorldPos0;
    
    OutNormal = Normal0 + texture(normalMap, TexCoord0).xyz;
    OutNormal = normalize(OutNormal); 
    
    OutDiffuse = texture(colorMap, TexCoord0).xyz + diffuseColor;

    OutTexCoord = vec3(OutTexCoord.xy, 1.0);
}