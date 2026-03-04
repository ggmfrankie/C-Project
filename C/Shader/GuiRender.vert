#version 460

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;

layout(location = 2) in int  inElementIndex;
layout(location = 3) in int  inAtlasIndex;


layout(std430, binding = 0) buffer InstanceBuffer {
    InstanceData instances[];
};


uniform float screenWidth;
uniform float screenHeight;

out vec2 outUV;
out vec4 outColor;
flat out int outHasTexture;
flat out int outAtlasIndex;
flat out float outBrightness;

struct InstanceData {
    float brightness;
    int hasTexture;
    vec2 worldPos;
    vec4 color;
};

void main() {
    InstanceData inst = instances[inElementIndex];

    vec2 wPos = pos + inst.worldPos;

    vec2 normPos = vec2((wPos.x / screenWidth) * 2.0 - 1.0, 1.0 - (wPos.y / screenHeight) * 2.0);

    gl_Position = vec4(normPos, 0.0, 1.0);

    outUV = uv;
    outColor = color;
    outHasTexture = inst.hasTexture;
    outBrightness = inst.brightness;
    outAtlasIndex = inAtlasIndex;
}
