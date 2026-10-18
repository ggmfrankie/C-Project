#version 460
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;

out vec2 texCoords;

uniform float screenWidth;
uniform float screenHeight;

uniform vec2 screenPos;
uniform vec2 screenDims;

vec4 calculateNormPos(vec2 relPos, vec2 worldPos) {
    vec2 wPos = relPos + worldPos;
    vec2 normPos = vec2((wPos.x / screenWidth) * 2.0 - 1.0, 1.0 - (wPos.y / screenHeight) * 2.0);
    return vec4(normPos, 0.0, 1.0);
}

void main() {

    gl_Position = calculateNormPos(pos * screenDims, screenPos);
    texCoords = uv;
}