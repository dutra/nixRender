
in vec3 position;
out vec2 fUV;

uniform sampler2D transmittanceSampler;

void main() {
    fUV = (position.xy + 1.0) / 2.0;
    
    gl_Position = vec4(position.xy, 0.0, 1.0);
}
