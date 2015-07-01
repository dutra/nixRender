
in vec2 TexCoord0; 
out vec4 outColor;

uniform sampler2D colorTex;

void main() {
		outColor = texture(colorTex, TexCoord0);
		//outColor = vec4(TexCoord0.xy, 1.0, 1.0);
}
