

#version 330 core


in vec3 vPosition_vs;
in vec3 vNormal_vs;

// uniform int uNumLights;  // Nombre de lumières
// uniform vec3 uLightPos[MAX_LIGHTS];  // Tableau de positions des lumières
// uniform vec3 uLightIntensity[MAX_LIGHTS];  // Tableau d'intensités des lumières

uniform vec3 uLightPos1;
uniform vec3 uLightPos2;

uniform vec3 uLightIntensity1;
uniform vec3 uLightIntensity2;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

out vec4 fFragColor;

vec3 blinnPhong(vec3 light_pos, vec3 light_intensity) {
    vec3 result = vec3(0.0);

        float d = distance(vPosition_vs, light_pos);
        vec3 Li = light_intensity / (d * d);
        vec3 N = vNormal_vs;
        vec3 w0 = normalize(-vPosition_vs);
        vec3 wi = normalize(light_pos - vPosition_vs);
        vec3 halfVector = normalize(w0 + wi);

        result += Li * (uKd * dot(wi, N) + uKs * pow(dot(halfVector, N), uShininess));
    

    return result;
}

void main() {
    fFragColor = vec4(blinnPhong(uLightPos1, uLightIntensity1)+ blinnPhong(uLightPos2, uLightIntensity2), 1.0);
}