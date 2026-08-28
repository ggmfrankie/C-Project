#version 460

in vec2 f_UV;
in vec4 f_Color;
flat in int f_AtlasID;

in vec4 fragColor;

layout(binding = 0) uniform sampler2D guiSampler;
layout(binding = 1) uniform sampler2D glyphSampler;

void main() {
    vec4 base;

    if (f_AtlasID == 0) base = texture(guiSampler, f_UV);
    else
    if (f_AtlasID == 1) base = texture(glyphSampler, f_UV);

    fragColor = f_Color * base;
}
