#version 460

in vec2 f_UV;
in vec4 f_Color;
flat in float f_Brightness;
flat in int f_HasTexture;
flat in int f_AtlasIndex;

out vec4 fragColor;

layout(binding = 0) uniform sampler2D guiSampler;
layout(binding = 1) uniform sampler2D glyphSampler;

void main() {
    vec4 base;
    if (f_HasTexture == 1){

        if (f_AtlasIndex == 0) {
            base = texture(guiSampler, f_UV);
        } else {
            base = texture(glyphSampler, f_UV);
        }

    } else {
        base = f_Color;
    }
    fragColor = (1.0 - f_Brightness) * base;
}
