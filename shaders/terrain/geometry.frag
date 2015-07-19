in vec3 fPosition; 
in vec3 fNormal; 

out vec3 pPosition; 
out vec3 pNormal; 
out vec3 pIdentifier;

void main() { 

    pPosition = fPosition;    
    pNormal = normalize(fNormal);
    pIdentifier = vec3(1.0/255, 0, 0);
    
}