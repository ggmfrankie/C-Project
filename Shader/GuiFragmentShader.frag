#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 texCoord;

out vec3 outPosition;

uniform float width;
uniform float height;

uniform float screenWidth;
uniform float screenHeight;

void main() {
    vec2 pixelPos = vec2(position.x * width, position.y * height);
    vec2 normPos = vec2((pixelPos.x / screenWidth) * 2.0 - 1.0, 1.0 - (pixelPos.y / screenHeight) * 2.0);
    gl_Position = vec4(normPos, 0.0, 1.0);
    outPosition = position;
}