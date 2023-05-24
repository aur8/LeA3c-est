

#version 330 core


in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTextCoords;

// uniform int uNumLights;  // Nombre de lumières
// uniform vec3 uLightPos[MAX_LIGHTS];  // Tableau de positions des lumières
// uniform vec3 uLightIntensity[MAX_LIGHTS];  // Tableau d'intensités des lumières

uniform vec3 uLightPos1;
uniform vec3 uLightPos2;
uniform vec3 uLightDir;

uniform vec3 uLightIntensity1;
uniform vec3 uLightIntensity2;
uniform vec3 uLightIntensity3;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

uniform sampler2D uTexture;

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

vec3 blinnPhong_dir(vec3 light_dir, vec3 light_intensity){
    vec3 Li = light_intensity;
    vec3 N = vNormal_vs;
    vec3 w0 = normalize(-vPosition_vs);
    vec3 wi = normalize(light_dir);
    vec3 halfVector = (w0+wi)/2.f;

    //la formule imbuvable du tp
    return Li * ( uKd * dot(wi,N) + uKs * pow(dot(halfVector, N), uShininess));
    // return Li*(uKd*max(dot(wi, N), 0.) + uKs*max(pow(dot(halfVector, N), 0.), uShininess));
}

void main() {
    vec2 fCorrectCoords = vec2(vTextCoords.x, 1-vTextCoords.y);

    vec3 lights = blinnPhong(uLightPos1, uLightIntensity1)+ blinnPhong(uLightPos2, uLightIntensity2) + blinnPhong_dir(uLightDir, uLightIntensity3);
    vec3 color_texture = texture(uTexture, fCorrectCoords).rgb;

    

    fFragColor = vec4( color_texture * lights , 1.0) ;

    //fFragColor = texture(uTexture, vTextCoords);
}