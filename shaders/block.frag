
//in vec3 Color;
//in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D texKitten;

void main() {
    outColor = vec4(0.6, 0.6, 0.6, 1.0);// * texture(texKitten, Texcoord);
    //	outColor = vec4(1.0, 0.5, 0.5, 1.0);
    //	outColor = vec4(Color, 1.0);
}
