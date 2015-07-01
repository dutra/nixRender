// second blur pass and add to original

in vec2 TexCoord0; 
out vec4 fragColor;

uniform int Width;

uniform sampler2D RenderTex;
uniform sampler2D BlurTex;

//uniform float PixOffset[10] =
//	float[](0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0);
uniform float Weight[20];
// 9.0	float[](0.1332, 0.126, 0.1066, 0.0808, 0.0548, 0.0332, 0.01802, 0.0088, 0.0038, 0.00148);
//	float[](0.075, 0.0739, 0.071, 0.06615, 0.06, 0.053, 0.0455, 0.038, 0.031, 0.0243, );

float LumThresh = 0.7;

float luma(vec3 color) {

	return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;

}

	
void main() {
	float dx = 1.0 / float(Width);
	vec4 val = texture(RenderTex, TexCoord0);
	vec4 sum = texture(BlurTex, TexCoord0) * Weight[0];
	for(int i=0; i<20; i++) {
		sum += texture( BlurTex, TexCoord0 +
						vec2(float(i), 0.0) * dx) * Weight[i];
		sum += texture( BlurTex, TexCoord0 -
						vec2(float(i), 0.0) * dx) * Weight[i];
	}
	if((luma(val.rgb) - LumThresh) > 0.0) {
		fragColor = val;
	} else {
		fragColor = min(val + sum, 1.0);
	
	}
	fragColor = vec4(fragColor.rgb, 1.0);
}
