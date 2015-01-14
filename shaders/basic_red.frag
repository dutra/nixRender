
in vec2 fUV;
out vec4 outColor;

uniform sampler2D texBlue;

void main() {
      outColor = texture2D(texBlue, fUV) + vec4(1.0, 0.0, 0.0, 1.0);
}
