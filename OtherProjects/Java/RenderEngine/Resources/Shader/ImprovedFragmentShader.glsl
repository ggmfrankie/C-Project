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
    vec4 emission;
    float specularExponent;
    float opticalDensity;
    float dissolve;
    int hasTexture;
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

void setupColors(Material material, vec2 textCoord) {
    if(material.hasTexture == 1) {
        vec4 texColor = texture(texture_sampler, textCoord);
        ambientC = texColor * material.ambient;
        diffuseC = texColor * material.diffuse;
        speculrC = texColor * material.specular;
    } else {
        ambientC = material.ambient;
        diffuseC = material.diffuse;
        speculrC = material.specular;
    }
}

// Calculates diffuse + specular contribution of a single light
vec4 calcLightColor(vec3 lightColor, float lightIntensity, vec3 position, vec3 toLightDir, vec3 normal) {
    vec4 diffuseColor = vec4(0);
    vec4 specColor = vec4(0);

    // Diffuse
    float diffuseFactor = max(dot(normal, toLightDir), 0.0);
    diffuseColor = diffuseC * vec4(lightColor,1.0) * lightIntensity * diffuseFactor;

    // Specular
    vec3 viewDir = normalize(cameraPos - position);
    vec3 reflectDir = reflect(-toLightDir, normal);
    float specFactor = pow(max(dot(viewDir, reflectDir), 0.0), material.specularExponent);
    specColor = speculrC * specFactor * vec4(lightColor,1.0) * lightIntensity;

    return diffuseColor + specColor;
}

// Calculates point or spot light with attenuation
vec4 calcLight(vec3 lightPosition, vec3 lightColor, float lightIntensity, vec3 position, vec3 normal, Attenuation att) {
    vec3 lightDir = lightPosition - position;
    float distance = length(lightDir);
    vec3 toLightDir = normalize(lightDir);

    vec4 lightCol = calcLightColor(lightColor, lightIntensity, position, toLightDir, normal);

    // Attenuation
    float attenuation = att.constant + att.linear * distance + att.exponent * distance * distance;
    return lightCol / attenuation;
}

vec4 calcPointLight(PointLight light, vec3 position, vec3 normal) {
    return calcLight(light.position, light.color, light.intensity, position, normal, light.att);
}

vec4 calcSpotLight(SpotLight light, vec3 position, vec3 normal) {
    vec3 lightDir = normalize(light.position - position);
    float spotFactor = dot(lightDir, normalize(-light.direction));
    if(spotFactor < cos(light.angle)) return vec4(0.0);
    return calcLight(light.position, light.color, light.intensity * spotFactor, position, normal, light.att);
}

vec4 calcDirectionalLight(DirectionalLight light, vec3 position, vec3 normal) {
    return calcLightColor(light.color, light.intensity, position, normalize(-light.direction), normal);
}

void main() {
    // Setup colors from material/texture
    setupColors(material, outTexCoord);

    // Ambient component
    vec4 ambientComponent = ambientC * vec4(ambientLight, 1.0);

    // Lighting contributions
    vec4 lighting = vec4(0.0);
    lighting += calcDirectionalLight(directionalLight, mvVertexPos, mvVertexNormal);
    lighting += calcPointLight(pointLight, mvVertexPos, mvVertexNormal);
    lighting += calcSpotLight(spotLight, mvVertexPos, mvVertexNormal);

    // Combine ambient, lighting, and emission
    fragColor = ambientComponent + lighting + material.emission;

    // Clamp to avoid overbright
    fragColor = clamp(fragColor, 0.0, 1.0);
}