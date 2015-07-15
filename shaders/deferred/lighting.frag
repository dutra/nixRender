
in vec2 TexCoord0; 

out vec4 OutColor; 

uniform sampler2D positionTex;
uniform sampler2D normalTex;
uniform sampler2D diffuseTex;

vec3 La = vec3(0.0, 0.0, 0.0);
vec3 Ld = vec3(1.0, 0.0, 0.0);

void main() { 
    vec3 lightPosition = vec3(0.0, 35.0, 0.0);
    vec3 Position = texture(positionTex, TexCoord0).xyz;
    vec3 n = texture(normalTex, TexCoord0).xyz;
    vec3 s = normalize(lightPosition - Position);
    vec3 v = normalize(vec3(-Position));
    vec3 r = reflect(-s, n);
    
    vec3 Ka = texture(diffuseTex, TexCoord0).rgb;
    
    vec3 Color = Ka*max(dot(s, n), 0.0)*Ld + Ka*La;
    OutColor = vec4(Color.rgb, 1.0);
    
    
    //OutColor = vec4(texture(normalTex, TexCoord0).xyz, 1.0) + 0.000000001*vec4(texture(positionTex, TexCoord0).xyz, 1.0) + 0.0000001*vec4(texture(diffuseTex, TexCoord0).xyz, 1.0);
}

