
in vec2 fUV; 

out vec4 pAmbient; 
out vec4 pDiffuse; 
out vec4 pSpecular; 

uniform sampler2D unTexPosition;
uniform sampler2D unTexNormal;
uniform sampler2D unTexIdentifier;


void main() { 
    
    // terrain
    if (texture(unTexIdentifier, fUV).r == (1.0/255)) {
        if (texture(unTexPosition, fUV).x < 128 && texture(unTexPosition, fUV).z < 128) {
            pAmbient = vec4(0.0, 0.0, 0.1, 1.0);
            pDiffuse = vec4(0.0, 0.0, 0.8, 1.0);
            pSpecular = vec4(0.0, 0.0, 1.0, 14.0);
        }
        else {
            pAmbient = vec4(0.0, 0.1, 0.0, 1.0);
            pDiffuse = vec4(0.0, 0.8, 0.0, 1.0);
            pSpecular = vec4(0.0, 1.0, 0.0, 14.0);

        }
    }
}