#version 460

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inUv;

layout(location = 2) in int  inBufferBinding;
layout(location = 3) in int  inIdx;

struct ElementInstanceData {
    vec4 color;
    vec2 worldPos;
    int atlasID;
};

struct CharInstanceData {
    vec4 color;
    int ownerID;
    int atlasID;
};

layout(std430, binding = 0) buffer InstanceBuffer {
    ElementInstanceData elements[];
};

layout(std430, binding = 1) buffer CharBuffer {
    CharInstanceData chars[];
};

uniform float screenWidth;
uniform float screenHeight;

out vec2 f_UV;
out vec4 f_Color;
flat out int f_AtlasID;

vec4 calculateNormPos(vec2 relPos, vec2 worldPos) {
    vec2 wPos = relPos + worldPos;
    vec2 normPos = vec2((wPos.x / screenWidth) * 2.0 - 1.0, 1.0 - (wPos.y / screenHeight) * 2.0);
    return vec4(normPos, 0.0, 1.0);
}

void handleElement() {
    ElementInstanceData inst = elements[inIdx];

    gl_Position = calculateNormPos(inPos, inst.worldPos);
    f_Color = inst.color;
    f_AtlasID = inst.atlasID;
}

void handleChar() {
    CharInstanceData inst = chars[inIdx];

    vec2 worldPos = elements[inst.ownerID].worldPos;
    gl_Position = calculateNormPos(inPos, worldPos);
    f_Color = inst.color;
    f_AtlasID = inst.atlasID;
}

void main() {

    if (inBufferBinding == 0) handleElement();
    else
    if (inBufferBinding == 1) handleChar();

    f_UV = inUv;
}
