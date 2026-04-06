#version 460
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;

out vec2 texCoords;

uniform float screenWidth;
uniform float screenHeight;

void main() {
    vec2 ndc = vec2(
        (pos.x / screenWidth) * 2.0 - 1.0,
        (pos.y / screenHeight) * 2.0 - 1.0
    );
    gl_Position = vec4(ndc, 0.0, 1.0);
    texCoords = uv;
}
