#version 460
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;

out vec2 texCoords;

uniform mat4 projectionMatrix;

void main() {
    gl_Position = projectionMatrix * vec4(pos, 0.0, 1.0);
    texCoords = uv;
}
