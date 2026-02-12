#version 460

in vec2 outUV;
in vec4 outColor;
flat in float outBrightness;
flat in int outHasTexture;

out vec4 fragColor;

uniform sampler2D atlasSampler;

void main() {
    if(outHasTexture == 1){
        vec4 texColor = texture(atlasSampler, outUV);
        fragColor = vec4(texColor.rgb, texColor.a);
    } else {
        fragColor = vec4(outColor.rgb, outColor.a);
    }
    //fragColor = vec4(0.3, 0.7, 0.9, 1.0);
}
