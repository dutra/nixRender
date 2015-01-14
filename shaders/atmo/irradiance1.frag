
// computes ground irradiance due to direct sunlight E[L0] (line 2 in algorithm 4.1)

out vec4 outColor;

void main() {
    float r, muS;
    getIrradianceRMuS(r, muS);
    outColor = vec4(transmittance(r, muS) * max(muS, 0.0), 0.0);
    //outColor = texture2D(transmittanceSampler, fUV);
    //outColor = vec4(1.0, 1.0, 0.0, 1.0);
    //outColor = vec4(fUV, 1.0, 1.0);
}