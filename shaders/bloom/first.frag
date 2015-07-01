// first blur pass

in vec2 TexCoord0; 
out vec4 fragColor;

uniform int Height;

uniform sampler2D BlurTex;

//uniform float PixOffset[20] =
//	float[](0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12);
uniform float Weight[20];
//	float[](0.075, 0.0739, 0.071, 0.06615, 0.06, 0.053, 0.0455, 0.038, 0.031, 0.0243);

void main() {
	float dy = 1.0 / float(Height);
	
	vec4 sum = texture(BlurTex, TexCoord0) * Weight[0];
	for(int i=0; i<20; i++) {
		sum += texture( BlurTex, TexCoord0 +
						vec2(0.0, float(i)) * dy) * Weight[i];
		sum += texture( BlurTex, TexCoord0 -
						vec2(0.0, float(i)) * dy) * Weight[i];
	}
	fragColor = sum;
	fragColor = min(vec4(fragColor.rgb, 1.0), 1.0);

}
