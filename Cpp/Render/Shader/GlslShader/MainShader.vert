#version 460
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 uv;

out vec2 texCoords;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

void main() {
    texCoords = uv;
    vec4 mvPos = modelViewMatrix * vec4(pos, 1.0);
    gl_Position = projectionMatrix * mvPos;
}
