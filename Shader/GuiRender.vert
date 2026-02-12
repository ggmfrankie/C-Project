#version 460

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec4 color;
layout (location = 3) in float brightness;
layout (location = 4) in int hasTexture;

uniform float screenWidth;
uniform float screenHeight;

out vec2 outUV;
out vec4 outColor;
out float outBrightness;
out int outHasTexture;


void main() {
    vec2 normPos = vec2((pos.x / screenWidth) * 2.0 - 1.0, 1.0 - (pos.y / screenHeight) * 2.0);

    gl_Position = vec4(normPos, 0.0, 1.0);
    outUV = uv;
    outHasTexture = hasTexture;
}
