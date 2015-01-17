
out vec4 outColor;

uniform float k; // k=0 for line 4, k=1 for line 10
uniform sampler2D deltaESampler;

void main() {
    vec2 uv = gl_FragCoord.xy / vec2(SKY_W, SKY_H);
    outColor = vec4(0.8, 0.3, 0.6, 1.0) + k * texture2D(deltaESampler, uv); // k=0 for line 4, k=1 for line 10
}
