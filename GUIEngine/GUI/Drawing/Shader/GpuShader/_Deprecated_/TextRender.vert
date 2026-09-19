#version 460

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inUV;

out vec2 outUV;

uniform float screenWidth;
uniform float screenHeight;

void main() {
    vec2 normPos = vec2((inPos.x / screenWidth) * 2.0 - 1.0, 1.0 - (inPos.y / screenHeight) * 2.0);
    gl_Position = vec4(normPos, 0.0, 1.0);
    outUV = inUV;
}
