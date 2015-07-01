
in vec2 TexCoord0; 
out vec4 fragColor;

uniform sampler2D colorTex;
float LumThresh = 0.70;

float luma(vec3 color) {

	return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;

}

void main() {

	vec4 val = texture(colorTex, TexCoord0);
	fragColor = val * clamp( luma(val.rgb) - LumThresh, 0.0, 1.0 ) * (1.0 / (1.0 - LumThresh));
	fragColor = vec4(fragColor.rgb, 1.0);
	//fragColor = min(fragColor, 1.0);
}
