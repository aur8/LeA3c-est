
struct PointLight {
    vec3 position; 

    float constant; 
    float linear;
    float quadratic;

    vec3 ambient; 
    vec3 quadratic; 
    vec3 specular;
};

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir = normalize(light.position - fragPos);

    //diffusion
    float diff = max(dot(normal, lightDir), 0.);

    //specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.), material.shininess);
}