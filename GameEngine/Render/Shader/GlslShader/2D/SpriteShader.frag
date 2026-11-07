#version 460

in vec2 texCoords;
out vec4 FragColor;

uniform vec4 color;

layout(binding = 0) uniform sampler2D textureSampler;

void main() {
    FragColor = texture(textureSampler, texCoords) * color;
}
