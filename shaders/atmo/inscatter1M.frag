
// computes ground irradiance due to direct sunlight E[L0] (line 2 in algorithm 4.1)

out vec4 outColor;

uniform float r;
uniform vec4 dhdH;
//uniform int layer;

void integrand(float r, float mu, float muS, float nu, float t, out vec3 mie) {
    
    mie = vec3(0.0);
    float ri = sqrt(r * r + t * t + 2.0 * r * mu * t);
    float muSi = (nu * t + muS * r) / ri;
    ri = max(Rg, ri);
    if (muSi >= -sqrt(1.0 - Rg * Rg / (ri * ri))) {
        vec3 ti = transmittance(r, mu, t) * transmittance(ri, muSi);
        
        mie = exp(-(ri - Rg) / HM) * ti;
    }
}

void inscatter(float r, float mu, float muS, float nu, out vec3 mie) {
    
    mie = vec3(0.0);
    float dx = limit(r, mu) / float(INSCATTER_INTEGRAL_SAMPLES);
    float xi = 0.0;
    
    vec3 miei;
    integrand(r, mu, muS, nu, 0.0, miei);
    for (int i = 1; i <= INSCATTER_INTEGRAL_SAMPLES; ++i) {
        float xj = float(i) * dx;
        
        vec3 miej;
        integrand(r, mu, muS, nu, xj, miej);
        
        mie += (miei + miej) / 2.0 * dx;
        xi = xj;
        
        miei = miej;
    }
    
    mie *= betaMSca;
}

void main() {
    
    vec3 mie;
    float mu, muS, nu;
    getMuMuSNu(r, dhdH, mu, muS, nu);
    inscatter(r, mu, muS, nu, mie);
    // store separately Rayleigh and Mie contributions, WITHOUT the phase function factor
    // (cf "Angular precision")
    
    outColor = vec4(mie, 1.0);
}