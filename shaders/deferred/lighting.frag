
in vec2 fTexCoord; 

out vec4 OutColor; 

uniform sampler2D positionTex;
uniform sampler2D normalTex;
/*
uniform sampler2D unAmbientTex;
uniform sampler2D unDiffuseTex;
uniform sampler2D unSpecularTex;
*/
vec3 lightPosition = vec3(0.0, 35.0, 0.0);
vec3 lightIntensity = vec3(1.0, 0.0, 0.0);

vec3 ads() {
    vec3 Position = texture(positionTex, fTexCoord).xyz;
    vec3 n = texture(normalTex, fTexCoord).xyz;
    vec3 s = normalize(lightPosition - Position);
    vec3 v = normalize(vec3(-Position));
    vec3 r = reflect(-s, n);
/*  
    vec3 Ka = texture(unAmbientTex, fTexCoord).rgb;
    vec3 Kd = texture(unDiffuseTex, fTexCoord).rgb;
    vec4 Ks = texture(unSpecularTex, fTexCoord).rgba;
*/
  
    vec3 Ka = vec3(0.0, 0.0, 0.0);
    vec3 Kd = vec3(1.0, 0.0, 0.0);
    vec4 Ks = vec4(0.0, 0.0, 0.0, 10.0);

 
 vec3 Color = lightIntensity *
                 (  Ka +
                    Kd * max(dot(s, n), 0.0) +
                    Ks.rgb * pow(max(dot(r, v), 0.0), Ks.a) );
    return Color;

}
void main() { 
    OutColor = vec4(ads(), 1.0);    
}

