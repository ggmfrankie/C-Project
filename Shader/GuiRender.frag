#version 460

in vec2 outUV;
in vec4 outColor;
in float outBrightness;
in int outHasTexture;

out vec4 fragColor;

uniform sampler2D texture_sampler;

void main() {
    if(outHasTexture == 1){
        vec4 texColor = texture(texture_sampler, outUV);
        fragColor = vec4(texColor.rgb * outBrightness, texColor.a * transparency);
    } else {
        fragColor = vec4(outColor.rgb * outBrightness, outColor.a);
    }
}
