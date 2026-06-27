#version 460

in vec2 outTexCoord;
in vec3 mvVertexNormal;
in vec3 mvVertexPos;

out vec4 fragColor;

struct Attenuation{
    float constant;
    float linear;
    float exponent;
};

struct PointLight{
    vec3 color;
    vec3 position;
    float intensity;
    Attenuation att;
};

struct SpotLight{
    vec3 color;
    vec3 position;
    float intensity;
    Attenuation att;
    float angle;
    vec3 direction;
};

struct Material{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    int hasTexture;
    float reflectance;
};

struct DirectionalLight
{
    vec3 color;
    vec3 direction;
    float intensity;
};

//uniform float uTime;
uniform vec3 ambientLight;
uniform float specularPower;
uniform Material material;
uniform PointLight pointLight;
uniform SpotLight spotLight;
uniform DirectionalLight directionalLight;
uniform vec3 cameraPos;

//uniform vec3 color;
//uniform int useColor;
uniform sampler2D texture_sampler;

vec4 ambientC;
vec4 diffuseC;
vec4 speculrC;

void setupColors            (Material material, vec2 textCoord);
vec4 calcLightColor         (vec3 lightColor, float lightIntensity, vec3 position, vec3 toLightDir, vec3 normal);
vec4 calcLight              (vec3 lightPosition, vec3 lightColor, float lightIntensity, vec3 position, vec3 normal, Attenuation att);
vec4 calcPointLight         (PointLight light, vec3 position, vec3 normal);
vec4 calcSpotLight          (SpotLight spotLight, vec3 position, vec3 normal);
vec4 calcDirectionalLight   (DirectionalLight light, vec3 position, vec3 normal);

//#define INTEGRATED_GPU
void main() {
    vec4 baseLight = vec4(0.2f, 0.2f, 0.2f, 1.0);
    setupColors(material, outTexCoord);
    
    vec4 diffuseSpecularComp = calcDirectionalLight(directionalLight, mvVertexPos, mvVertexNormal);
    diffuseSpecularComp += calcPointLight(pointLight, mvVertexPos, mvVertexNormal);
    diffuseSpecularComp += calcSpotLight(spotLight, mvVertexPos, mvVertexNormal);

    fragColor = ambientC * vec4(ambientLight, 1.0) + diffuseSpecularComp;

    #ifdef INTEGRATED_GPU
    fragColor = vec4(mvVertexNormal * 0.5 + 0.5, 1.0);
    #endif
}

void setupColors(Material material, vec2 textCoord){
    if(material.hasTexture == 1){
        ambientC = texture(texture_sampler, textCoord);
        diffuseC = ambientC;
        speculrC = ambientC;
    }
    else{
        ambientC = material.ambient;
        diffuseC = material.diffuse;
        speculrC = material.specular;
    }
}
vec4 calcLightColor(vec3 lightColor, float lightIntensity, vec3 position, vec3 toLightDir, vec3 normal){
    vec4 diffuseColor = vec4(0, 0, 0, 0);
    vec4 specColor = vec4(0, 0, 0, 0);

    // Diffuse Light
    float diffuseFactor = max(dot(normal, toLightDir), 0.0);
    diffuseColor = diffuseC * vec4(lightColor, 1.0) * lightIntensity * diffuseFactor;

    // Specular Light
    vec3 cameraDirection = normalize(cameraPos - position);
    vec3 fromLightDir = -toLightDir;
    vec3 reflectedLight = normalize(reflect(fromLightDir , normal));
    float specularFactor = max( dot(cameraDirection, reflectedLight), 0.0);
    specularFactor = pow(specularFactor, specularPower);
    specColor = speculrC * lightIntensity  * specularFactor * material.reflectance * vec4(lightColor, 1.0);

    return (diffuseColor + specColor);
}

vec4 calcLight(vec3 lightPosition, vec3 lightColor, float lightIntensity, vec3 position, vec3 normal, Attenuation att){
    vec3 lightDirection = lightPosition - position;
    vec3 toLightDir = normalize(lightDirection);
    vec4 lightColorOut = calcLightColor(lightColor, lightIntensity, position, toLightDir, normal);

    // Attenuation
    float distance = length(lightDirection);
    float attenuationInv = att.constant + att.linear * distance + att.exponent * distance * distance;
    return lightColorOut / attenuationInv;
}

vec4 calcPointLight(PointLight light, vec3 position, vec3 normal){
    return calcLight(light.position, light.color, light.intensity, position, normal, light.att);
}

vec4 calcSpotLight(SpotLight light, vec3 position, vec3 normal){
    vec3 lightDirection = normalize(light.position - position);
    if(dot(lightDirection, light.direction) < light.angle){
        return vec4(0, 0, 0, 0);
    }
    return calcLight(light.position, light.color, light.intensity, position, normal, light.att);
}

vec4 calcDirectionalLight(DirectionalLight light, vec3 position, vec3 normal){
    return calcLightColor(light.color, light.intensity, position, normalize(light.direction), normal);
}




