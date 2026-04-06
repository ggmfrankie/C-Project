#version 460
in vec2 texCoords;

out vec4 FragColor;

uniform vec3 color;

layout(binding = 0) uniform sampler2D textureSampler;

void main() {
    FragColor = vec4(color, 1.0);
}
