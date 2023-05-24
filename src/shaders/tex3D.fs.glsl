#version 330 core

in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTextCoords;

uniform sampler2D uTexture;

out vec4 fFragColor;

void main(){
    vec2 fCorrectCoords = vec2(vTextCoords.x, 1-vTextCoords.y);
    //fFragColor = vec4(vec3(normalize(vNormal_vs)), 1.);

    fFragColor = texture(uTexture, fCorrectCoords);
}


