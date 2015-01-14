
// computes ground irradiance due to direct sunlight E[L0] (line 2 in algorithm 4.1)

out vec4 outColor;

uniform float r;
uniform vec4 dhdH;
//uniform int layer;

void integrand(float r, float mu, float muS, float nu, float t, out vec3 ray) {
    ray = vec3(0.0);
    
    float ri = sqrt(r * r + t * t + 2.0 * r * mu * t);
    float muSi = (nu * t + muS * r) / ri;
    ri = max(Rg, ri);
    if (muSi >= -sqrt(1.0 - Rg * Rg / (ri * ri))) {
        vec3 ti = transmittance(r, mu, t) * transmittance(ri, muSi);
        ray = exp(-(ri - Rg) / HR) * ti;
    
    }
}

void inscatter(float r, float mu, float muS, float nu, out vec3 ray) {
    ray = vec3(0.0);
    float dx = limit(r, mu) / float(INSCATTER_INTEGRAL_SAMPLES);
    float xi = 0.0;
    vec3 rayi;
    integrand(r, mu, muS, nu, 0.0, rayi);
    for (int i = 1; i <= INSCATTER_INTEGRAL_SAMPLES; ++i) {
        float xj = float(i) * dx;
        vec3 rayj;
        integrand(r, mu, muS, nu, xj, rayj);
        ray += (rayi + rayj) / 2.0 * dx;
        
        xi = xj;
        rayi = rayj;
        
    }
    ray *= betaR;
    
}

void main() {
    vec3 ray;
    
    float mu, muS, nu;
    getMuMuSNu(r, dhdH, mu, muS, nu);
    inscatter(r, mu, muS, nu, ray);
    // store separately Rayleigh and Mie contributions, WITHOUT the phase function factor
    // (cf "Angular precision")
    outColor = vec4(ray, 1.0);
    //outColor = vec4(ray, 1.0);
    //outColor = vec4(1.0, 1.0, 0.0, 1.0);
    //outColor = vec4(fUV, 1.0, 1.0);
    //gl_FragData[1].rgb = mie;
}