#version 460

in vec2 texCoords;
in vec3 outNorm;

out vec4 FragColor;

layout(binding = 0) uniform sampler2D textureSampler;

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

void main() {
    FragColor = texture(textureSampler, texCoords);
}
