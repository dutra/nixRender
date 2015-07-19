
in vec2 fTexCoord; 

out vec4 OutColor; 

uniform sampler2D unTexPosition;
uniform sampler2D unTexNormal;
uniform sampler2D unTexIdentifier;

uniform sampler2D unTexAmbient;
uniform sampler2D unTexDiffuse;
uniform sampler2D unTexSpecular;

vec3 lightPosition = vec3(0.0, 35.0, 0.0);
vec3 lightIntensity = vec3(1.0, 1.0, 1.0);

vec3 ads() {

    vec3 Ka = texture(unTexAmbient, fTexCoord).rgb;
    vec3 Kd = texture(unTexDiffuse, fTexCoord).rgb;
    vec4 Ks = texture(unTexSpecular, fTexCoord).rgba;


    vec3 Position = texture(unTexPosition, fTexCoord).xyz;
    vec3 n = texture(unTexNormal, fTexCoord).xyz;
    vec3 s = normalize(lightPosition - Position);
    vec3 v = normalize(vec3(-Position));
    vec3 r = reflect(-s, n);

 
 vec3 Color = lightIntensity *
                 (  Ka +
                    Kd * max(dot(s, n), 0.0) +
                    Ks.rgb * pow(max(dot(r, v), 0.0), Ks.a) );
    return Color;

}
void main() { 
    OutColor = vec4(ads(), 1.0);    
}

