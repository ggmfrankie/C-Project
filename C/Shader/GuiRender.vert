#version 460

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;

layout(location = 2) in int  inElementIndex;
layout(location = 3) in int  inAtlasIndex;

struct InstanceData {
    float brightness;
    int hasTexture;
    vec2 worldPos;
    vec4 color;
};

layout(std430, binding = 0) buffer InstanceBuffer {
    InstanceData instances[];
};

uniform float screenWidth;
uniform float screenHeight;

out vec2 f_UV;
out vec4 f_Color;
flat out int f_HasTexture;
flat out int f_AtlasIndex;
flat out float f_Brightness;

void main() {
    InstanceData inst = instances[inElementIndex];

    vec2 wPos = pos + inst.worldPos;

    vec2 normPos = vec2((wPos.x / screenWidth) * 2.0 - 1.0, 1.0 - (wPos.y / screenHeight) * 2.0);

    gl_Position = vec4(normPos, 0.0, 1.0);

    f_UV = uv;
    f_Color = inst.color;
    f_HasTexture = inst.hasTexture;
    f_Brightness = inst.brightness;
    f_AtlasIndex = inAtlasIndex;
}
